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

SET (ACME_VERSION "v0.4.0")

MESSAGE(STATUS "ACME Version: ${ACME_VERSION}")

IF ("${TARGET_OS}" STREQUAL "")
	SET(TARGET_OS ${CMAKE_SYSTEM_NAME})
ENDIF()

add_definitions("-DTARGET_OS=${TARGET_OS}")

IF("${TARGET_ARCH}" STREQUAL "")
	IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i386")
		SET(TARGET_ARCH "X86_32")
	ELSEIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
		SET(TARGET_ARCH "X86_32")
	ELSEIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
		SET(TARGET_ARCH "X86_64")
	ELSEIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm") # this is for yocto build
		SET(TARGET_ARCH "ARMV7L") # fall back to ARMV7L
	ELSE()
		SET(TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
	ENDIF()
ENDIF()

add_definitions("-DTARGET_ARCH=${TARGET_ARCH}")
add_definitions("-DARCH_${TARGET_ARCH}")

MESSAGE(STATUS "Using TARGET_OS = ${TARGET_OS}")
MESSAGE(STATUS "Using TARGET_ARCH = ${TARGET_ARCH}")

IF(NOT GLOBAL_TOP_LEVEL_SOURCE_DIR)
	SET(GLOBAL_TOP_LEVEL_SOURCE_DIR ${PROJECT_SOURCE_DIR} CACHE PATH "top level source path")
ENDIF()

IF(NOT GLOBAL_TOP_LEVEL_BINARY_DIR)
	SET(GLOBAL_TOP_LEVEL_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE PATH "top level build path")
ENDIF()

SET(GLOBAL_EXTERNAL_LIBRARIES_INSTALL_DIR "${GLOBAL_TOP_LEVEL_BINARY_DIR}/externals")

# Initialize CMAKE variables

IF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Build = 'Debug' as default.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "BUILD_TYPE" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
ENDIF()

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    IF("${TARGET_OS}" STREQUAL "Windows")
        SET(CMAKE_INSTALL_PREFIX "${GLOBAL_TOP_LEVEL_SOURCE_DIR}/deliverable/${TARGET_OS}_${TARGET_ARCH}/\${BUILD_TYPE}" CACHE PATH "install directory used by install, default: /usr/local on UNIX and c:/Program Files on Windows" FORCE )
    ELSE() 
        SET(CMAKE_INSTALL_PREFIX "${GLOBAL_TOP_LEVEL_SOURCE_DIR}/deliverable/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}" CACHE PATH "install directory used by install, default: /usr/local on UNIX and c:/Program Files on Windows" FORCE )
    ENDIF()
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

IF("${TARGET_OS}" STREQUAL "Windows")
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_CFG_INTDIR}/bin" CACHE INTERNAL "path to store executable files")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_CFG_INTDIR}/lib" CACHE INTERNAL "path to store library files")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_CFG_INTDIR}/lib" CACHE INTERNAL "path to store archive files")
    SET(CMAKE_RESOURCE_OUTPUT_DIRECTORY "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_CFG_INTDIR}/res" CACHE INTERNAL "path to store resource files")
    
    FOREACH( CONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string( TOUPPER ${CONFIG} CONFIG_UPPER )
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CONFIG}/bin" CACHE INTERNAL "path to store executable files")
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CONFIG}/lib" CACHE INTERNAL "path to store library files")
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CONFIG}/lib" CACHE INTERNAL "path to store archive files")
    ENDFOREACH()
ELSE()                                                                                                                   
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/bin" CACHE INTERNAL "path to store executable files")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/lib" CACHE INTERNAL "path to store library files")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/lib" CACHE INTERNAL "path to store archive files")
    SET(CMAKE_RESOURCE_OUTPUT_DIRECTORY "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/res" CACHE INTERNAL "path to store resource files")
ENDIF()
SET(CMAKE_HEADER_OUTPUT_DIRECTORY   "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/include"            			  CACHE INTERNAL "path to store public header files")
SET(CMAKE_DOC_OUTPUT_DIRECTORY      "${GLOBAL_TOP_LEVEL_BINARY_DIR}/${CMAKE_PROJECT_NAME}/doc"                			  CACHE INTERNAL "path to store documentation files")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
	
# Define global cache-variables and set them to their default values

SET(CONFIG_VERBOSE	 0  CACHE BOOL     "enables a more verbose output of acme process")   
SET(CONFIG_BUILD_UNITTESTS 					 1 	CACHE BOOL 	   "building Unit-Tests")
SET(CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE    	 0  CACHE BOOL     "enable building of one test executable")

SET(GLOBAL_HEADER_FILE_EXTENSIONS 	"h;hpp;inc" 	CACHE STRING "file extension of header files.")
SET(GLOBAL_SOURCE_FILE_EXTENSIONS 	"cpp;c;def;rc"  CACHE STRING "file extension of source files.")
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

SET(GLOBAL_UTILS_MODULES_STATIC  		"" CACHE INTERNAL "stores the module names of all static modules")
SET(GLOBAL_UTILS_MODULES_DYNAMIC 		"" CACHE INTERNAL "stores the module names of all dynamic modules")
SET(GLOBAL_UTILS_MODULES_EXE     		"" CACHE INTERNAL "stores the module names of all exe modules")
SET(GLOBAL_UTILS_MODULES_TESTS   		"" CACHE INTERNAL "stores the module names of all tests modules")

SET(GLOBAL_MODULE_NAMES					"" CACHE INTERNAL "global list of all module names")

SET(GLOBAL_CMAKE_PROJECTS                   "" CACHE INTERNAL "global list of all added cmake project")
SET(GLOBAL_DEPENDENCY_EDGES "" CACHE INTERNAL "global list of all dependencies that are used to generate the dependency graph") 