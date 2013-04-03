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

IF(NOT DEFINED THIRD_PARTY_DIR)
	SET(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3psw)
ENDIF()


include(ExternalProject)
find_package(PkgConfig)

MESSAGE(STATUS "Loading plugins")

FILE(GLOB_RECURSE ACME_PLUGINS ${ACME_PATH}/plugins/*.cmake)
FOREACH(plugin ${ACME_PLUGINS})
	MESSAGE(VERBOSE "Found acme plugin ${plugin}")
	INCLUDE(${plugin})
ENDFOREACH()

FILE(GLOB_RECURSE EXTERN_PLUGINS ${CMAKE_SOURCE_DIR}/cmake/plugins/*.cmake)
FOREACH(plugin ${EXTERN_PLUGINS})
	MESSAGE(VERBOSE "Found internal plugin ${plugin}")
	GET_FILENAME_COMPONENT(PLUGIN_NAME ${plugin} NAME_WE)
	STRING(TOUPPER ${PLUGIN_NAME} PLUGIN_NAME)
	INCLUDE(${plugin})
ENDFOREACH()
