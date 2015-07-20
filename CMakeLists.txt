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

SET(ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND LD_LIBRARY_PATH=../DynamicTestLibrary && ${CMAKE_CTEST_COMMAND};--target test -V CACHE STRING "")
SET(CMAKE_MODULE_PATH
    ${CMAKE_INSTALL_PREFIX}/lib/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/acme2
)

FIND_PACKAGE(ACME2)
FIND_PACKAGE(automotive-dlt)

IF(MSVC)
    SET(ACME_ENABLE_PLUGIN_VISUAL_STUDIO_PDB true CACHE BOOL "ACME2 plugin for installing Visual Studio PDB(s)")
ENDIF()

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
    VERSION_MAJOR               0
    VERSION_MINOR               19
    VERSION_PATCH               0
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

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/CapuConfigVersion.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CapuConfigVersion.cmake"
    @ONLY
)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/CapuConfigVersion.cmake
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/cmake
)
ACME_INFO("G CapuBuildConfig.cmake")

#==============================================================================
# generate and install CMake Find Module file
#==============================================================================

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/FindCapu.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FindCapu.cmake"
    @ONLY
)
INSTALL(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/FindCapu.cmake
    DESTINATION ${PROJECT_INSTALL_STATIC_LIB}/cmake
)
ACME_INFO("G FindCapu.cmake")

#==============================================================================
# generate and install DltLogAppenderBuildConfig header
#==============================================================================
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/DltLogAppenderBuildConfig.h.in"
    "${CMAKE_BINARY_DIR}/BuildConfig/DltLogAppenderBuildConfig.h"
    @ONLY
)

ACME_INFO("G DltLogAppenderBuildConfig.h")
