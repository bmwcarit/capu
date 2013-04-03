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

IF(NOT ${CONFIG_BUILD_UNITTESTS})
	UNSET(BUILD_GoogleMock CACHE)
ELSE()
	IF("${CONFIG_GTEST_URL}" STREQUAL "")
		SET(CONFIG_GTEST_URL "http://googlemock.googlecode.com/files/gmock-1.6.0.zip")
	ENDIF()

	SET(NEEDED_LIBRARIES "Thread")
	IF(NOT "${TARGET_OS}" STREQUAL Windows)
		SET(NEEDED_LIBRARIES ${NEEDED_LIBRARIES} Rt)
	ENDIF()
	ACME_ADD_CMAKE_PROJECT(	GoogleMock 
						LIBNAMES gmock gtest
						LIBDIRS "src/GoogleMock-build" "src/GoogleMock-build/gtest"
						INCLUDE_DIRS "src/GoogleMock/include" "src/GoogleMock/gtest/include"
						URL "${CONFIG_GTEST_URL}"
						CHECKSUM "f547f47321ca88d3965ca2efdcc2a3c1"
						CMAKE_ARGUMENTS "gtest_force_shared_crt:Bool=1"
						REQUIRED_PACKAGES "${NEEDED_LIBRARIES}"
						)

ENDIF()

