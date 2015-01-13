############################################################################
#
# Copyright (C) 2014 BMW Car IT GmbH
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

SET(ACMEPLUGIN_LINUX_TESTCOVERAGE_TESTCOMMAND ${CMAKE_CTEST_COMMAND};--target test -V CACHE STRING     "default: run tests with ctest")

SET(ACMEPLUGIN_LINUX_TESTCOVERAGE_REMOVE_LCOV -r combined.info */gtest/* -r combined.info */gmock/* CACHE STRING     "")

SET(ACMEPLUGIN_LINUX_TESTCOVERAGE_REMOVE_GCOV ;-e '.*/gtest/';-e '.*/gmock/' CACHE STRING     "")

IF("${TARGET_OS}" STREQUAL "Linux")
	IF(NOT LCOV_FOUND)
		FIND_PROGRAM(LCOV_PATH lcov)
	ENDIF()
ENDIF()
