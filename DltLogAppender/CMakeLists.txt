#
# Copyright (C) 2015 BMW Car IT GmbH
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

#determine if log_level_ptr in DltContext can be used
#is available in dlt >= 2.10
IF(automotive-dlt_FOUND)

IF(${automotive-dlt_VERSION} STREQUAL  2.11.1)
    ADD_DEFINITIONS(-DDLT_CONTEXT_USE_LOG_LEVEL_PTR)
ELSEIF(${automotive-dlt_VERSION} STREQUAL  2.11.0)
    ADD_DEFINITIONS(-DDLT_CONTEXT_USE_LOG_LEVEL_PTR)
ELSEIF(${automotive-dlt_VERSION} STREQUAL  2.10.0)
    ADD_DEFINITIONS(-DDLT_CONTEXT_USE_LOG_LEVEL_PTR)
ENDIF()

ENDIF()

ACME_MODULE(

    #==========================================================================
    # general module information
    #==========================================================================
    NAME                    DltLogAppender
    TYPE                    STATIC_LIBRARY

    #==========================================================================
    # files of this module
    #==========================================================================
    FILES_PUBLIC_HEADER     include/DltLogAppender/*.h

    FILES_SOURCE            src/DltLogAppender/*.cpp

    FILES_DOCUMENTATION     doc/*

    INCLUDE_BASE            include

    #==========================================================================
    # dependencies
    #==========================================================================
    DEPENDENCIES            Capu
                            automotive-dlt
)
