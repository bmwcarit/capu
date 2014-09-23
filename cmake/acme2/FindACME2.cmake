############################################################################
#
# Copyright 2014 BMW Car IT GmbH
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

# ACME2 path already defined
IF (NOT "${ACME2_BASE_DIR}" STREQUAL "")
    RETURN()
ENDIF()

# try to resolve global ACME2 dir from environment
IF ("${ACME2_BASE_DIR}" STREQUAL "")
    SET(ACME2_BASE_DIR $ENV{ACME2_BASE_DIR})
ENDIF()

# use local installation
IF ("${ACME2_BASE_DIR}" STREQUAL "")
    SET(ACME2_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
ENDIF()

IF("${ACME2_BASE_DIR}" STREQUAL "")
    MESSAGE(STATUS "ACME2 not found. Please provide source dir of ACME2 using ACME2_BASE_DIR.")
    SET(ACME2_BASE_DIR "ACME2_BASE_DIR-NOTFOUND")
ELSE()
    MESSAGE(STATUS "ACME2 found: ${ACME2_BASE_DIR}")
    INCLUDE(${ACME2_BASE_DIR}/acme2.cmake)
    SET(ACME2_FOUND TRUE)
ENDIF()
