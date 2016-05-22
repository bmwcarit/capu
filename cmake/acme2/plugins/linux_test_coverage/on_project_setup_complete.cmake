############################################################################
#
# Copyright (C) 2014 BMW Car IT GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

FUNCTION(ADD_TESTCOVERAGE_TARGETS name dependency)
    ADD_CUSTOM_COMMAND(OUTPUT CreateInitialCoverage_${name}
        COMMAND lcov --zerocounters --directory ${CMAKE_BINARY_DIR} --output-file everything_initial_${name}.info --quiet
        COMMAND lcov --capture --initial --directory ${CMAKE_BINARY_DIR} --output-file everything_initial_${name}.info --quiet
        DEPENDS ${dependency}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
        COMMENT "Codecoverage (${name}): Initialize everything to 0"
    )
    ADD_CUSTOM_COMMAND(OUTPUT RUNTESTS_${name}
        COMMAND ${ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND_${name}}
        DEPENDS CreateInitialCoverage_${name}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Codecoverage (${name}): Run tests to gather coverage information"
    )
    message(${ACMEPLUGIN_LINUX_TESTCOVERAGE_EXCLUDE_${NAME}}="${ACMEPLUGIN_LINUX_TESTCOVERAGE_EXCLUDE_${NAME}}")
    ADD_CUSTOM_COMMAND(OUTPUT CreateTestCoverage_Data_${name}
        COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --kernel-directory ${CMAKE_BINARY_DIR} --output-file everything_afterRun_${name}.info
                 --base-directory ${CMAKE_SOURCE_DIR} --no-external --quiet
        COMMAND lcov --add-tracefile everything_initial_${name}.info --add-tracefile everything_afterRun_${name}.info --output-file combined_${name}.info  --quiet
        COMMAND lcov
         --remove combined_${name}.info /usr/*.* ${ACMEPLUGIN_LINUX_TESTCOVERAGE_EXCLUDE_${NAME}}
         --output-file combinedFiltered_${name}.info --quiet --base-directory ${CMAKE_SOURCE_DIR} --no-external
        DEPENDS RUNTESTS_${name}
        COMMENT "Codecoverage (${name}): Crunch coverage data"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
    )

    ADD_CUSTOM_COMMAND(OUTPUT CreateTestCoverage_${name}
        COMMAND genhtml -s -l combinedFiltered_${name}.info --quiet --output-directory html_${name}
        DEPENDS CreateTestCoverage_Data_${name}
        COMMENT "Codecoverage (${name}): Generate html"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
    )

    IF (NOT TARGET CreateTestCoverage${name})
        ADD_CUSTOM_TARGET(CreateTestCoverage${name}
            COMMENT "Creating TestCoverage"
            DEPENDS CreateTestCoverage_${name}
        )
    ENDIF()
ENDFUNCTION()

IF("${TARGET_OS}" STREQUAL "Linux")
	IF(NOT LCOV_FOUND)
		FIND_PROGRAM(LCOV_PATH lcov)
	ENDIF()
ENDIF()

ADD_CUSTOM_COMMAND(OUTPUT CreateCoverageDirectory
    COMMAND rm
        ARGS -rf CodeCoverage
    COMMAND mkdir
        ARGS "CodeCoverage"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating CodeCoverage directory"
)

SET(DEPENDENCY CreateCoverageDirectory)
FOREACH(NAME ${ACMEPLUGIN_LINUX_TESTCOVERAGE_NAMES})
   IF(ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND_${NAME})
      IF(ACMEPLUGIN_LINUX_TESTCOVERAGE_EXCLUDE_${NAME})
        MESSAGE("Adding test coverage run for ${NAME}")
        ADD_TESTCOVERAGE_TARGETS(${NAME} ${DEPENDENCY})
        SET(DEPENDENCY CreateTestCoverage${NAME})
      ELSE()
        MESSAGE("Must provide ACMEPLUGIN_LINUX_TESTCOVERAGE_EXCLUDE_${NAME}")
      ENDIF()
   ELSE()
       MESSAGE("Must provide ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND_${NAME}")
  ENDIF()
ENDFOREACH()

IF (NOT TARGET CreateTestCoverage)
    ADD_CUSTOM_TARGET(CreateTestCoverage
        COMMENT "Creating TestCoverage"
        DEPENDS ${DEPENDENCY}
    )
ENDIF()
