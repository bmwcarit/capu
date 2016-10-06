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
    SET(PLATFORM_SPECIFIC_SOURCE_FILES ${PLATFORM_SPECIFIC_SOURCE_FILES} src/os/MacOSX/*.cpp src/os/iPhoneOS/*.cpp)
    SET(PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES} include/capu/os/MacOSX/*.h include/capu/os/iPhoneOS/*.h)
ENDIF()

include(CheckIncludeFiles)
check_include_files(stdint.h CAPU_HAVE_STDINT_H)
check_include_files(stddef.h CAPU_HAVE_STDDEF_H)

include(CheckTypeSize)
check_type_size("char" CAPU_SIZEOF_CHAR BUILTIN_TYPES_ONLY)
check_type_size("short int" CAPU_SIZEOF_SHORT_INT BUILTIN_TYPES_ONLY)
check_type_size("int" CAPU_SIZEOF_INT BUILTIN_TYPES_ONLY)
check_type_size("long long int" CAPU_SIZEOF_LONG_LONG_INT BUILTIN_TYPES_ONLY)
check_type_size("long int" CAPU_SIZEOF_LONG_INT BUILTIN_TYPES_ONLY)
check_type_size("__int64" CAPU___INT64 BUILTIN_TYPES_ONLY)
check_type_size(int8_t CAPU_INT8_T)
check_type_size(int16_t CAPU_INT16_T)
check_type_size(int32_t CAPU_INT32_T)
check_type_size(int64_t CAPU_INT64_T)
check_type_size(uint8_t CAPU_UINT8_T)
check_type_size(uint16_t CAPU_UINT16_T)
check_type_size(uint32_t CAPU_UINT32_T)
check_type_size(uint64_t CAPU_UINT64_T)

CONFIGURE_FILE(include/capu/Config.h.in ${PROJECT_BINARY_DIR}/capu/Config.h)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

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
                            ${PROJECT_BINARY_DIR}/capu/*.h
                            ${PLATFORM_SPECIFIC_PUBLIC_HEADER_FILES}


    FILES_SOURCE            src/os/${TARGET_OS}/*.cpp
                            src/util/*.cpp
                            src/os/Generic/*.cpp
                            ${PLATFORM_SPECIFIC_SOURCE_FILES}

    FILES_PRIVATE_HEADER    include/capu/Config.h.in

    FILES_DOCUMENTATION     doc/*

    INCLUDE_BASE            include
                            ${PROJECT_BINARY_DIR}

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

# set defines public to target
TARGET_COMPILE_DEFINITIONS(Capu PUBLIC
    TARGET_OS=${TARGET_OS}
    TARGET_ARCH=${TARGET_ARCH}
    ${CAPU_OS_COMPILE_DEFINITION}
    ${CAPU_ARCH_COMPILE_DEFINITION})


IF(Capu_BUILD_TESTS AND NOT "${TARGET_OS}" STREQUAL "Integrity")

    ACME_MODULE(

        #==========================================================================
        # general module information
        #==========================================================================
        NAME                    DynamicTestLibrary
        TYPE                    SHARED_LIBRARY

        #==========================================================================
        # files of this module
        #==========================================================================
        INCLUDE_BASE            test/DynamicTestLibrary/include
        FILES_PRIVATE_HEADER    test/DynamicTestLibrary/include/*.h

        FILES_SOURCE            test/DynamicTestLibrary/src/*.cpp

        #==========================================================================
        # dependencies
        #==========================================================================
        DEPENDENCIES_HEADER     Capu
    )
ENDIF()

ACME_MODULE(

   #==========================================================================
   # general module information
   #==========================================================================
   NAME                    Capu_Test
   TYPE                    TEST

   #==========================================================================
   # files of this module
   #==========================================================================
   INCLUDE_BASE            test/src
   FILES_SOURCE            test/src/*.h
                           test/src/*.cpp
                           test/src/container/*.h
                           test/src/container/*.cpp
                           test/src/os/*.h
                           test/src/os/*.cpp
                           test/src/util/*.h
                           test/src/util/*.cpp

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

