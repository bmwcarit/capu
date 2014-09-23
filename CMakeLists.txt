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

#set default install prefix to stay backward compatible with version 0.13.0
SET (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deliverable/${TARGET_OS}_${TARGET_ARCH}/${CMAKE_BUILD_TYPE})

SET(CMAKE_MODULE_PATH
    ${CMAKE_INSTALL_PREFIX}/lib/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/acme2
)

FIND_PACKAGE(ACME2)

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
    VERSION_MINOR               13
    VERSION_PATCH               0
    DESCRIPTION                 "Collections and Platform Utils"
    EMAIL                       "martin.veith@bmw-carit.de"
    COMPANY                     "BMW Car IT GmbH"

    #==========================================================================
    # project content
    #==========================================================================
    CONTENT                     ./Capu
                                ./DynamicTestLibrary
                                ./Capu_Test
)
