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

FIND_PROGRAM(QT5_PATH qmake)

SET(QT5_PATH ${QT5_PATH} CACHE PATH "")

IF("${QT5_PATH}" STREQUAL "QT5_PATH-NOTFOUND")
	MESSAGE(STATUS "   The executable of the additional software \"Qt5\" was not found!")
	MESSAGE(STATUS "   If you have installed \"Qt5\" then add the path to the executable of \"Qt5\" into the Cache Variable \"QT5_PATH\".")
	MESSAGE(STATUS "   Otherwise ACME will not perform the memory check!")
	MESSAGE(STATUS)
ELSE()
	MESSAGE(STATUS "   Found the additional software ${QT5_PATH}")
	SET(QT5_FOUND 1 CACHE INTERNAL "")
ENDIF()
