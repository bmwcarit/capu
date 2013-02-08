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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8) # required for target property 'include_directory'

IF(NOT GLOBAL_TOP_LEVEL_SOURCE_DIR)
	SET(GLOBAL_TOP_LEVEL_SOURCE_DIR ${PROJECT_SOURCE_DIR} CACHE PATH "top level source path")
ENDIF()

IF(NOT GLOBAL_TOP_LEVEL_BINARY_DIR)
	SET(GLOBAL_TOP_LEVEL_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE PATH "top level build path")
ENDIF()

# Initialize CMAKE variables

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	#SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/deliverable/${CMAKE_PROJECT_NAME}" CACHE PATH "install directory used by install, default: /usr/local on UNIX and c:/Program Files on Windows" FORCE )
	SET(CMAKE_INSTALL_PREFIX "${GLOBAL_TOP_LEVEL_SOURCE_DIR}/deliverable" CACHE PATH "install directory used by install, default: /usr/local on UNIX and c:/Program Files on Windows" FORCE )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

IF("${GLOBAL_TOP_LEVEL_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
	IF("${TARGET_OS}" STREQUAL "Windows")
		SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/bin/${TARGET_OS}_${TARGET_ARCH}" CACHE INTERNAL "path to store executable files")
		SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/lib/${TARGET_OS}_${TARGET_ARCH}" CACHE INTERNAL "path to store library files")
		SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/lib/${TARGET_OS}_${TARGET_ARCH}" CACHE INTERNAL "path to store archive files")
	ELSE()
		SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/bin/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "path to store executable files")
		SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/lib/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "path to store library files")
		SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/lib/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "path to store archive files")
	ENDIF()
	SET(CMAKE_HEADER_OUTPUT_DIRECTORY   "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/include"            			  CACHE INTERNAL "path to store public header files")
	SET(CMAKE_RESOURCE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/res"                			  CACHE INTERNAL "path to store resource files")
	SET(CMAKE_DOC_OUTPUT_DIRECTORY      "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/doc"                			  CACHE INTERNAL "path to store documentation files")
ENDIF()
	
# Define global cache-variables and set them to their default values

SET(CONFIG_DETAILED_CONFIGURATION_REPORT	 0  CACHE BOOL     "enables a more detailed configuration report")   
SET(CONFIG_BUILD_UNITTESTS 					 1 	CACHE BOOL 	   "building Unit-Tests")
SET(CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE    	 0  CACHE BOOL     "enable building of one test executable")

SET(GLOBAL_HEADER_FILE_EXTENSIONS 	"h;hpp;inc" 	CACHE STRING "file extension of header files.")
SET(GLOBAL_SOURCE_FILE_EXTENSIONS 	"cpp;c" 		CACHE STRING "file extension of source files.")
SET(GLOBAL_EXTERN_INCLUDE_DIRS      ""  			CACHE INTERNAL "collect extern include dirs")

SET(GLOBAL_TEST_LIBS                ""  			CACHE INTERNAL "collect test libs")
SET(GLOBAL_TEST_SOURCE              ""  			CACHE INTERNAL "collect test source")
SET(GLOBAL_TEST_INCLUDE_DIRECTORIES ""  			CACHE INTERNAL "collect test include directories")
SET(GLOBAL_TEST_DEBUG_COMPILER_FLAGS      	""	CACHE INTERNAL "collect test compiler flags")
SET(GLOBAL_TEST_DEBUG_LINKER_FLAGS        	""  CACHE INTERNAL "collect test linker flags")
SET(GLOBAL_TEST_DEBUG_DEFINITIONS         	""  CACHE INTERNAL "collect test defintions")
SET(GLOBAL_TEST_RELEASE_COMPILER_FLAGS    	""  CACHE INTERNAL "collect test compiler flags")
SET(GLOBAL_TEST_RELEASE_LINKER_FLAGS      	""  CACHE INTERNAL "collect test linker flags")
SET(GLOBAL_TEST_RELEASE_DEFINITIONS      	""  CACHE INTERNAL "collect test defintions")
SET(GLOBAL_TEST_LINKER_DIRECTORIES  		""  CACHE INTERNAL "collect test linker directories")

#SET(GLOBAL_INCLUDE_DIRECTORIES     		""  CACHE INTERNAL "collect include directories")
#SET(GLOBAL_LIB_DIRECTORIES          		""  CACHE INTERNAL "collect lib directories")
#SET(GLOBAL_LIBRARIES                		""  CACHE INTERNAL "collect all linkable libraries")

SET(GLOBAL_UTILS_MODULES_STATIC  		"" CACHE INTERNAL "stores the module names of all static modules")
SET(GLOBAL_UTILS_MODULES_DYNAMIC 		"" CACHE INTERNAL "stores the module names of all dynamic modules")
SET(GLOBAL_UTILS_MODULES_EXE     		"" CACHE INTERNAL "stores the module names of all exe modules")
SET(GLOBAL_UTILS_MODULES_TESTS   		"" CACHE INTERNAL "stores the module names of all tests modules")

#SET(GLOBAL_OPTIONAL_FILES 				"" CACHE INTERNAL "global list of all variables which indicate whether an optional file is enabled")

SET(GLOBAL_MODULE_NAMES					"" CACHE INTERNAL "global list of all module names")
SET(GLOBAL_MODULE_DEPENDENCIES 			"" CACHE INTERNAL "global list of all variables which store the dependencies of the specific module")
SET(GLOBAL_MODULE_LIBRARIES 			"" CACHE INTERNAL "global list of all variables which store the libraries of the specific module")
SET(GLOBAL_MODULE_HAS_SOURCE_FILES 		"" CACHE INTERNAL "global list of all variables which indentifiy if the specific module has source files")
SET(GLOBAL_MODULE_TYPE 					"" CACHE INTERNAL "global list of all variables which store the type of the specific module")
SET(GLOBAL_MODULE_DIR 					"" CACHE INTERNAL "global list of all variables which store the directory of the specific module")
SET(GLOBAL_MODULE_BUILD_ENABLED 		"" CACHE INTERNAL "global list of all variables which show if build is for the specific module enabled")
#SET(GLOBAL_MODULE_COMPILE_FLAGS 		"" CACHE INTERNAL "global list of all variables which store the compile flags for the specific module")
SET(GLOBAL_MODULE_DEBUG_COMPILER_FLAGS 	"" CACHE INTERNAL "global list of all variables which store the debug compiler flags for the specific module")
SET(GLOBAL_MODULE_RELEASE_COMPILER_FLAGS "" CACHE INTERNAL "global list of all variables which store the release compiler flags for the specific module")
#SET(GLOBAL_MODULE_LINKER_FLAGS 			"" CACHE INTERNAL "global list of all variables which store the linker flags for the specific module")
SET(GLOBAL_MODULE_DEBUG_LINKER_FLAGS 	"" CACHE INTERNAL "global list of all variables which store the debug linker flags for the specific module")
SET(GLOBAL_MODULE_RELEASE_LINKER_FLAGS 	"" CACHE INTERNAL "global list of all variables which store the release linker flags for the specific module")
SET(GLOBAL_MODULE_DEBUG_DEFINITIONS 	"" CACHE INTERNAL "global list of all variables which store the debug definitions for the specific module")
SET(GLOBAL_MODULE_RELEASE_DEFINITIONS 	"" CACHE INTERNAL "global list of all variables which store the release definitions for the specific module")
SET(GLOBAL_MODULE_INSTALL_FILES 		"" CACHE INTERNAL "global list of all variables which identificate the install files for the specific module")
SET(GLOBAL_MODULE_PACKAGE_LIBS 			"" CACHE INTERNAL "global list of all variables which identificate the package libaries for the specific module")
SET(GLOBAL_MODULE_FOUND 				"" CACHE INTERNAL "global list of all variables which show that a specific module was found")
SET(GLOBAL_MODULE_TEST_FILES			"" CACHE INTERNAL "global list of all variables which store the test files of the specific module")
SET(GLOBAL_MODULE_SOURCE_FILES			"" CACHE INTERNAL "global list of all variables which store the source files of the specific module")
SET(GLOBAL_MODULE_OPTIONAL_FILES 		"" CACHE INTERNAL "global list of variables which indicate if an optional file ist enabled or not")

SET(GLOBAL_WITH_MODULE 					"" CACHE INTERNAL "global list of all used modules")
SET(GLOBAL_BUILD_PROJECT 				"" CACHE INTERNAL "global list of all build variables of the added cmake projekts")

SET(GLOBAL_EXTERNAL_LIBRARY_INCLUDE_DIR						"" CACHE INTERNAL "global list of all variables which indicate the include directory of external libraries")
SET(GLOBAL_EXTERNAL_LIBRARY_LIBRARIES_DIR					"" CACHE INTERNAL "global list of all variables which indicate the library directory of external libraries")
SET(GLOBAL_EXTERNAL_LIBRARY_LIBRARIES						"" CACHE INTERNAL "global list of all variables which indicate the libraries of external libraries")
SET(GLOBAL_EXTERNAL_LIBRARY_DEPENDENT_DEBUG_DEFINITIONS		"" CACHE INTERNAL "global list of all variables which indicate the dependent debug definitions of external libraries")
SET(GLOBAL_EXTERNAL_LIBRARY_DEPENDENT_RELEASE_DEFINITIONS	"" CACHE INTERNAL "global list of all variables which indicate the dependent release definitions of external libraries")
SET(GLOBAL_EXTERNAL_LIBRARY_FOUND							"" CACHE INTERNAL "global list of all variables which indicate if an external library was found")
SET(GLOBAL_EXTERNAL_LIBRARY_INTERNAL						"" CACHE INTERNAL "")

SET(GLOBAL_PACKAGE_FOUND			""	CACHE INTERNAL	"global list of all variables which identificate the found packages")
SET(GLOBAL_PACKAGE_INCLUDE_DIRS		"" 	CACHE INTERNAL	"global list of all variables which store the include directories of the found packages")
SET(GLOBAL_PACKAGE_LIBRARIES		""	CACHE INTERNAL	"global list of all variables which store the libraries of the found packages")

SET(GLOBAL_CMAKE_PROJECTS                   "" CACHE INTERNAL "global list of all added cmake project")
SET(GLOBAL_CMAKE_PROJECTS_REQUIRED_PACKAGES "" CACHE INTERNAL "global list of all required packages that are used by a specific external cmake project")
SET(GLOBAL_DEPENDENCY_EDGES "" CACHE INTERNAL "global list of all dependencies that are used to generate the dependency graph") 


# Reset used files

IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
	FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
ENDIF()

IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
	FILE(REMOVE ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
ENDIF()

IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DoitHooksTemp.cmake)
	FILE(REMOVE ${CMAKE_BINARY_DIR}/Hooks/DoitHooksTemp.cmake)
ENDIF()