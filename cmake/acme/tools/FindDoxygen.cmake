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

INCLUDE(FindDoxygen)

SET(DOXYGEN_FOUND ${DOXYGEN_FOUND} CACHE INTERNAL "")
SET(DOXYGEN_DOT_FOUND ${DOXYGEN_DOT_FOUND} CACHE INTERNAL "")

IF("${DOXYGEN_EXECUTABLE}" STREQUAL "DOXYGEN_EXECUTABLE-NOTFOUND")
	MESSAGE(STATUS "   The executable of the additional software \"doxygen\" was not found!")
	MESSAGE(STATUS "   If you have installed \"doxygen\" then add the path to the executable into the Cache Variable \"DOXYGEN_EXECUTABLE\".")
	MESSAGE(STATUS "   Otherwise ACME will not create the build report!")
ELSE()
	MESSAGE(STATUS "Found the additional software ${DOXYGEN_EXECUTABLE}")
ENDIF()