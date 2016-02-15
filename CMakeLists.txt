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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(CMakePackageConfigHelpers)

SET(ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND LD_LIBRARY_PATH=../DynamicTestLibrary && ${CMAKE_CTEST_COMMAND};--target test -V CACHE STRING "")
SET(CMAKE_MODULE_PATH
    ${CMAKE_INSTALL_PREFIX}/lib/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/acme2
)

FIND_PACKAGE(ACME2)

# Add option to enable/disable DLT usage and try to find it if requested
if (NOT DEFINED Capu_USE_DLT)
    ACME_INFO("Capu_USE_DLT flag not set. DLT support will be built if available.")
    SET(Capu_USE_DLT ON CACHE BOOL "Use DLT if available: ON, OFF" FORCE)
endif()

IF (Capu_USE_DLT)
    FIND_PACKAGE(automotive-dlt)
ENDIF()

IF(MSVC)
    SET(ACME_ENABLE_PLUGIN_VISUAL_STUDIO_PDB true CACHE BOOL "ACME2 plugin for installing Visual Studio PDB(s)")
ENDIF()

# version information
SET(Capu_VERSION_MAJOR 0)
SET(Capu_VERSION_MINOR 24)
SET(Capu_VERSION_PATCH 0)
SET(Capu_VERSION "${Capu_VERSION_MAJOR}.${Capu_VERSION_MINOR}.${Capu_VERSION_PATCH}")

# custom setup for integrating gtest & gmock
set(gtest_force_shared_crt true CACHE BOOL "Must be enabled for linking our unit tests" FORCE)
IF(Capu_BUILD_TESTS AND NOT TARGET gmock)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/external/gmock)
ENDIF()

ACME2_PROJECT(
    #==========================================================================
    # project metadata
    #==========================================================================
    NAME                        Capu
    VERSION_MAJOR               ${Capu_VERSION_MAJOR}
    VERSION_MINOR               ${Capu_VERSION_MINOR}
    VERSION_PATCH               ${Capu_VERSION_PATCH}
    DESCRIPTION                 "Collections and Platform Utils"
    EMAIL                       "martin.veith@bmw-carit.de"
    COMPANY                     "BMW Car IT GmbH"

    #==========================================================================
    # documentation files, will be installed and packaged
    #==========================================================================
    FILES_DOCUMENTATION         README.mdown
                                LICENSE.txt

    #==========================================================================
    # project content
    #==========================================================================
    CONTENT                     ON ./Capu
                                ON ./DynamicTestLibrary
                                ON ./Capu_Test
                                AUTO ./DltLogAppender
                                AUTO ./DltLogAppender_Test
)

#==============================================================================
# generate and install PkgConfig file
#==============================================================================

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/Capu.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Capu.pc"
    @ONLY
)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/Capu.pc
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/pkgconfig
)
ACME_INFO("G Capu.pc")

#==============================================================================
# generate and install CMake Version file
#==============================================================================

WRITE_BASIC_PACKAGE_VERSION_FILE("${CMAKE_CURRENT_BINARY_DIR}/CapuConfigVersion.cmake"
  VERSION ${Capu_VERSION}
  COMPATIBILITY AnyNewerVersion)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/CapuConfigVersion.cmake
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/cmake
)
ACME_INFO("G CapuConfigVersion.cmake")

#==============================================================================
# generate and install CMake Find Module file
#==============================================================================

# Use same CapuTemplate.cmake.in as base for Find* and *Config script.
# (Set manually the pattern variables for Find* that the function for *Config uses)

SET(PACKAGE_INIT)
SET(PACKAGE_PROJECT_INSTALL_STATIC_LIB "${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_STATIC_LIB}")
SET(PACKAGE_PROJECT_INSTALL_HEADER "${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_HEADER}")
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/CapuTemplate.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FindCapu.cmake"
    @ONLY
)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/FindCapu.cmake
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/cmake
)
ACME_INFO("G FindCapu.cmake")

#==============================================================================
# generate and install CMake Config file
#==============================================================================

CONFIGURE_PACKAGE_CONFIG_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/CapuTemplate.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CapuConfig.cmake"
    INSTALL_DESTINATION "${PROJECT_INSTALL_STATIC_LIB}/cmake"
    PATH_VARS PROJECT_INSTALL_HEADER PROJECT_INSTALL_STATIC_LIB PROJECT_INSTALL_SHARED_LIB
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/CapuConfig.cmake
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/cmake
)
ACME_INFO("G CapuConfig.cmake")

#==============================================================================
# generate and install DltLogAppenderBuildConfig header
#==============================================================================
# ensure variable is always set
IF (NOT DEFINED automotive-dlt_BUILD_CONFIG)
    SET(automotive-dlt_BUILD_CONFIG 0)
ENDIF()

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/DltLogAppenderBuildConfig.h.in"
    "${CMAKE_BINARY_DIR}/BuildConfig/DltLogAppenderBuildConfig.h"
    @ONLY
)

install(
    FILES "${CMAKE_BINARY_DIR}/BuildConfig/DltLogAppenderBuildConfig.h"
    DESTINATION "${PROJECT_INSTALL_HEADER}/DltLogAppender"
)

ACME_INFO("G DltLogAppenderBuildConfig.h")
