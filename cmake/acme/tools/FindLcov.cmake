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

FIND_PROGRAM(LCOV_PATH lcov)

SET(LCOV_PATH ${LCOV_PATH} CACHE PATH "")

IF("${LCOV_PATH}" STREQUAL "LCOV_PATH-NOTFOUND")
	MESSAGE(WARNING "   The executable of the additional software \"Lcov\" was not found!")
	MESSAGE(WARNING "   If you have installed \"Lcov\" then add the path to the executable of \"lcov\" into the Cache Variable \"LCOV_PATH\".")
	MESSAGE(WARNING "   Otherwise ACME will not create the test coverage!")
	MESSAGE(WARNING)
ELSE()
	MESSAGE(VERBOSE "   Found the additional software ${LCOV_PATH}")
	SET(LCOV_FOUND 1 CACHE INTERNAL "")
ENDIF()