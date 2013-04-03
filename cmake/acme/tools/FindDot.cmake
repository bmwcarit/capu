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

IF("${DOXYGEN_DOT_EXECUTABLE}" STREQUAL "DOXYGEN_DOT_EXECUTABLE-NOTFOUND")
	MESSAGE(WARNING "   The executable of the additional software \"dot\" was not found!")
	MESSAGE(WARNING "   If you have installed \"dot\" then add the path to the executable of \"dot\" into the Cache Variable \"DOXYGEN_DOT_EXECUTABLE\".")
	MESSAGE(WARNING "   Otherwise ACME will not create the dependency graph!")
	MESSAGE(WARNING)
ELSE()
	MESSAGE(VERBOSE "   Found the additional software ${DOXYGEN_DOT_EXECUTABLE}")
ENDIF()