############################################################################
#
# Copyright (C) 2014 BMW Car IT GmbH
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

FUNCTION(ACMEPLUGIN_BUILD_DOCUMENTATION)
	FILE(GLOB_RECURSE doc_files "${CMAKE_CURRENT_SOURCE_DIR}/doc/*.*") # TODO how to keep the file directory structure?
		SOURCE_GROUP("Documentation files" FILES ${doc_files})

		IF(ACMEPLUGIN_OPEN_DOC_ON_BUILD)
			IF("${TARGET_OS}" STREQUAL "Windows")
				SET(OPEN_INDEX_FILE START /B "${CMAKE_CURRENT_BINARY_DIR}/documentation/html/index.html") # async start
			ELSE()
				SET(OPEN_INDEX_FILE "") # unset for other os systems
			ENDIF()
		ELSE()
			SET(OPEN_INDEX_FILE "")
		ENDIF()

		# configure doxyfile with cmake variables
		configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

		ADD_CUSTOM_COMMAND(OUTPUT CreateDocumentationDirectory
        COMMAND ${CMAKE_COMMAND}
            ARGS -E make_directory documentation
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating Documentation directory"
        )

		ADD_CUSTOM_COMMAND(OUTPUT RunDoxyGen
        COMMAND ${DOXYGEN_EXECUTABLE}
            ARGS "${CMAKE_BINARY_DIR}/Doxyfile"
            DEPENDS CreateDocumentationDirectory
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating Documentation via doxygen"
        )

		# add the target
		add_custom_target(Documentation_${PROJECT_NAME}
			COMMAND ${OPEN_INDEX_FILE} # will be empty and do nothing by default
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Generating documentation" VERBATIM
			SOURCES ${doc_files}
			DEPENDS RunDoxyGen
		)
ENDFUNCTION(ACMEPLUGIN_BUILD_DOCUMENTATION)


SET(ACMEPLUGIN_DOC 0)
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
	SET(ACMEPLUGIN_DOC 1)
ENDIF()

IF(ACMEPLUGIN_DOC)
	IF(NOT DOXYGEN_FOUND)
		INCLUDE(FindDoxygen)
		SET(DOXYGEN_FOUND ${DOXYGEN_FOUND} CACHE INTERNAL "")
		SET(DOXYGEN_DOT_FOUND ${DOXYGEN_DOT_FOUND} CACHE INTERNAL "")
	ENDIF()
	ACMEPLUGIN_BUILD_DOCUMENTATION()
ENDIF()

