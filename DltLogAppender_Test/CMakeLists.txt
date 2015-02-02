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

ACME_MODULE(

    #==========================================================================
    # general module information
    #==========================================================================
    NAME                    DltLogAppender_Test
    TYPE                    TEST

    #==========================================================================
    # files of this module
    #==========================================================================
    INCLUDE_BASE            src
    FILES_SOURCE            src/*.h
                            src/*.cpp

    #==========================================================================
    # dependencies
    #==========================================================================
    DEPENDENCIES            DltLogAppender
                            gmock_main
)

FIND_PACKAGE(automotive-dlt)
IF(automotive-dlt_FOUND)
    ##only change linker flags if target is available due to automotive-dlt being present
    IF("${TARGET_OS}" STREQUAL "Integrity")
        GET_TARGET_PROPERTY(PREVIOUSFLAGS DltLogAppender_Test LINK_FLAGS )
        IF(NOT "${PREVIOUSFLAGS}")
            SET(PREVIOUSFLAGS "")
        ENDIF()
        SET_TARGET_PROPERTIES(DltLogAppender_Test PROPERTIES LINK_FLAGS "${PREVIOUSFLAGS} ${INTEGRITY_LD_FILES_FOLDER}/kernel_default.ld -kernel -lutil")
    ENDIF()
ENDIF()