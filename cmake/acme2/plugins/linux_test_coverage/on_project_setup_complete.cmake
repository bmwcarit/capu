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

FUNCTION(ADD_TESTCOVERAGE_TARGETS)
IF (NOT TARGET CreateTestCoverage)
    ADD_CUSTOM_COMMAND(OUTPUT DeleteCoverageDirectory
        COMMAND rm
            ARGS -rf CodeCoverage
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Removing old coverage directory"
        )

    ADD_CUSTOM_COMMAND(OUTPUT CreateCoverageDirectory
        COMMAND mkdir
            ARGS "CodeCoverage"
            DEPENDS DeleteCoverageDirectory
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating CodeCoverage directory"
        )

    ADD_CUSTOM_COMMAND(OUTPUT InitializeCoverageToZero
        COMMAND lcov
            ARGS
                -c
                -i
                -d ${CMAKE_BINARY_DIR}
                -o everything_initial.info
                -q
            DEPENDS CreateCoverageDirectory
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
            COMMENT "Initializing coverage to zero"
        )

    ADD_CUSTOM_COMMAND(OUTPUT RunTests
        COMMAND ${ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND}
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            DEPENDS InitializeCoverageToZero
            COMMENT "Running tests"
        )

    ADD_CUSTOM_COMMAND(OUTPUT CollectCoverage
        COMMAND lcov
            ARGS -c
                 -d ${CMAKE_BINARY_DIR}
                 -k ${CMAKE_BINARY_DIR}
                 -o everything_afterRun.info
                 --base-directory ${CMAKE_SOURCE_DIR}
                 --no-external
                 -q
             WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
             COMMENT "Collecting coverage result"
             DEPENDS RunTests
        )

    ADD_CUSTOM_COMMAND(OUTPUT AddZerosToResult
        COMMAND lcov
            ARGS -a everything_initial.info
                 -a everything_afterRun.info
                 -o combined.info
                 -q
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
            COMMENT "Adding zeros to result"
            DEPENDS CollectCoverage
        )

    ADD_CUSTOM_COMMAND(OUTPUT FilterCoverageResults
        COMMAND lcov
            ARGS -r combined.info ${CMAKE_SOURCE_DIR}/external/*.*
                 -r combined.info /usr/*.*
                 -r combined.info */test/*
                 ${ACMEPLUGIN_LINUX_TESTCOVERAGE_REMOVE_LCOV}
                 -o combinedFiltered.info
                 -q
                 -base-directory ${CMAKE_SOURCE_DIR} --no-external
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
            COMMENT "Filtering coverage results"
            DEPENDS AddZerosToResult
        )

    ADD_CUSTOM_COMMAND(OUTPUT GenerateHTML
        COMMAND genhtml
            ARGS -s
                 -l combinedFiltered.info
                 -q
                 --output-directory html
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/CodeCoverage"
            COMMENT "Creating HTML"
            DEPENDS FilterCoverageResults
        )

    ADD_CUSTOM_TARGET(CreateTestCoverage
        DEPENDS
            GenerateHTML
        COMMENT "Creating TestCoverage"
        )
ENDIF()
ENDFUNCTION(ADD_TESTCOVERAGE_TARGETS)

IF("${TARGET_OS}" STREQUAL "Linux") # only on linux because of gcov/lcov
    IF(NOT LCOV_FOUND)
        FIND_PROGRAM(LCOV_PATH lcov)
    ENDIF()
    ADD_TESTCOVERAGE_TARGETS()
ENDIF()

