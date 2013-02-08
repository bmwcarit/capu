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

#--------------------------------------------------------------------------
# Plugin API
#--------------------------------------------------------------------------

SET(CONFIG_CREATE_TEST_COVERAGE_BUILD_SERVER 0  CACHE BOOL     "sets gcov flags to create a test coverage report on the build server")


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_CREATE_TEST_COVERAGE_BUILD_SERVER)
	ADD_PLUGIN_DOIT_HOOK(INTERNAL_TEST_COVERAGE_ON_BUILD_SERVER\(\))
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_TEST_COVERAGE_ON_BUILD_SERVER)    
        IF("${TARGET_OS}" STREQUAL "Linux")
            IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") # otherwise, windows configure fails because CMAKE_BUILD_TYPE is not set
                ACME_REQUIRED_PACKAGE(Rt)
                ACME_LINK_LIBRARY(gcov)
                ACME_ADD_DEBUG_COMPILER_FLAG(-fprofile-arcs)
                ACME_ADD_DEBUG_COMPILER_FLAG(-ftest-coverage)
            ELSE()
                MESSAGE(STATUS "WARNING: The cache variable \"CMAKE_BUILD_TYPE\" ist not \"Debug\". To set gcov flags set this cache variable to \"Debug\".")
            ENDIF()
        ENDIF()
ENDFUNCTION()


