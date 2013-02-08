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


# This module looks for cppcheck and defines the following values:
#  CPPCHECK_EXECUTABLE: the full path to the cppcheck executable.
#  CPPCHECK_FOUND: True if cppcheck has been found.

FIND_PROGRAM(CPPCHECK_EXECUTABLE
  cppcheck
  /usr/bin
  C:/Program Files (x86)/Cppcheck
  C:/Program Files/Cppcheck
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppCheck DEFAULT_MSG CPPCHECK_EXECUTABLE)
MARK_AS_ADVANCED( CPPCHECK_EXECUTABLE )
SET(CPPCHECK_EXECUTABLE ${CPPCHECK_EXECUTABLE} CACHE PATH "")

IF(NOT "${CPPCHECK_EXECUTABLE}" STREQUAL "CPPCHECK_EXECUTABLE-NOTFOUND")
	SET(CPPCHECK_FOUND 1 CACHE INTERNAL "")
ENDIF()








