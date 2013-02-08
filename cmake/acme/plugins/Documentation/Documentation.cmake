#
# Copyright (C) 2012 BMW Car IT GmbH
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

# Adds a Documentation target to the project, if a "doc" folder exists.
# This doc folder must contain a doxygen configuration named Doxyfile.in
# The documentation will be generated into the deliverable folder.


#--------------------------------------------------------------------------
# Plugin API
#--------------------------------------------------------------------------

SET(CONFIG_OPEN_DOCUMENTATION_FILE 0 CACHE BOOL "immediately opening index.html when building documentation")
MARK_AS_ADVANCED(CONFIG_OPEN_DOCUMENTATION_FILE)
SET(CONFIG_DOCUMENTATION_FOLDER_NAME "documentation" CACHE STRING "folder name for the documentation folder")
MARK_AS_ADVANCED(CONFIG_DOCUMENTATION_FOLDER_NAME)


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

PRINT_DETAILS(STATUS "...requires external tool Doxygen")
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
	IF(NOT DOXYGEN_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindDoxygen.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
	#find_package(Doxygen)
	IF(DOXYGEN_FOUND)
		# show the sources in the target
		FILE(GLOB_RECURSE doc_files "${CMAKE_CURRENT_SOURCE_DIR}/doc/*.*") # TODO how to keep the file directory structure?
		SOURCE_GROUP("Documentation files" FILES ${doc_files})

		IF(CONFIG_OPEN_DOCUMENTATION_FILE)
			IF("${TARGET_OS}" STREQUAL "Windows")
				SET(OPEN_INDEX_FILE START /B "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_DOCUMENTATION_FOLDER_NAME}/html/index.html") # async start
			ELSE()
				SET(OPEN_INDEX_FILE "") # untests for other os systems
			ENDIF()
		ELSE()
			SET(OPEN_INDEX_FILE "")
		ENDIF()

		# configure doxyfile with cmake variables
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

		# add the target
		add_custom_target(Documentation
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			COMMAND ${OPEN_INDEX_FILE} # will be empty and do nothing by default
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating documentation" VERBATIM
			SOURCES ${doc_files}
		)
	ENDIF()
ENDIF()
