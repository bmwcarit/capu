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

# all POSIX system must include this source file,
# which currently refers to all supported platforms
# except windows

ACME_MODULE(

    #==========================================================================
    # general module information
    #==========================================================================
    NAME                    Capu_Test
    TYPE                    TEST

    #==========================================================================
    # files of this module
    #==========================================================================
    INCLUDE_BASE            src
    FILES_SOURCE            src/*.h
                            src/*.cpp
                            src/container/*.h
                            src/container/*.cpp
                            src/os/*.h
                            src/os/*.cpp
                            src/util/*.h
                            src/util/*.cpp

    #==========================================================================
    # dependencies
    #==========================================================================
    DEPENDENCIES            Capu
                            gmock_main
    DEPENDENCIES_HEADER     DynamicTestLibrary
)

IF(TARGET Capu_Test AND "${TARGET_OS}" STREQUAL "Integrity")
    GET_TARGET_PROPERTY(PREVIOUSFLAGS Capu_Test COMPILE_FLAGS )
    IF(NOT "${PREVIOUSFLAGS}")
        SET(PREVIOUSFLAGS "")
    ENDIF()
    SET_TARGET_PROPERTIES(Capu_Test PROPERTIES COMPILE_FLAGS "${PREVIOUSFLAGS} -no_misalign_pack")
ENDIF()
