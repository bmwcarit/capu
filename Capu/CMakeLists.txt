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
IF (NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(PLATFORM_SPECIFIC_SOURCE_FILES src/os/Posix/*.cpp)
    SET(PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES include/capu/os/Posix/*.h) 
ENDIF()

IF (TARGET_OS MATCHES "iPhoneOS")
    SET(PLATFORM_SPECIFIC_SOURCE_FILES ${PLATFORM_SPECIFIC_SOURCE_FILES} src/os/MacOSX/*.cpp)
    SET(PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES} include/capu/os/MacOSX/*.h)
ENDIF()

IF (TARGET_OS MATCHES "iPhoneSimulatorOS")
    SET(PLATFORM_SPECIFIC_SOURCE_FILES ${PLATFORM_SPECIFIC_SOURCE_FILES} src/os/MacOSX/*.cpp)
    SET(PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES} include/capu/os/MacOSX/*.h)
    SET(PLATFORM_SPECIFIC_SOURCE_FILES ${PLATFORM_SPECIFIC_SOURCE_FILES} src/os/iPhoneOS/*.cpp)
    SET(PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES} include/capu/os/iPhoneOS/*.h)
ENDIF()

ACME_MODULE(

    #==========================================================================
    # general module information
    #==========================================================================
    NAME                    Capu
    TYPE                    STATIC_LIBRARY

    #==========================================================================
    # files of this module
    #==========================================================================
    FILES_PUBLIC_HEADER     include/capu/*.h
                            include/capu/container/*.h
                            include/capu/os/*.h
                            include/capu/os/Generic/*.h
                            include/capu/os/${TARGET_OS}/*.h
                            include/capu/os/${TARGET_OS}/${TARGET_ARCH}/*.h
                            include/capu/util/*.h
                            ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES}


    FILES_SOURCE            src/os/${TARGET_OS}/*.cpp
                            src/util/*.cpp
                            src/os/Generic/*.cpp
                            ${PLATFORM_SPECIFIC_SOURCE_FILES}

    FILES_DOCUMENTATION     doc/*

    INCLUDE_BASE            include

    #==========================================================================
    # dependencies
    #==========================================================================
    DEPENDENCIES            Thread
                            Rt
                            Socket
                            Net
                            IP4
                            Res
                            DynamicLibrary
                            psapi
                            Pipe
)
