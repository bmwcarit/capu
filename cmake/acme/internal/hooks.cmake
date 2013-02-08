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
# Hooks within the function INTERNAL_FINALIZE
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_FINALIZE_HOOK functionname)
	FILE(APPEND "${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake" "${functionname}()\n")
ENDFUNCTION(ADD_PLUGIN_FINALIZE_HOOK)

FUNCTION(CALL_PLUGIN_FINALIZE_HOOKS)
	IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
		INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
	ENDIF()
ENDFUNCTION(CALL_PLUGIN_FINALIZE_HOOKS)


#--------------------------------------------------------------------------
# Hooks within the function INTERNAL_ADD_DEPENDENCY
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_DEPENDENCY_HOOK functionname)
	FILE(APPEND "${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake" "${functionname}\n")
ENDFUNCTION(ADD_PLUGIN_DEPENDENCY_HOOK)

FUNCTION(CALL_PLUGIN_DEPENDENCY_HOOKS)
	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
		INCLUDE(${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
	ENDIF()
ENDFUNCTION(CALL_PLUGIN_DEPENDENCY_HOOKS)


#--------------------------------------------------------------------------
# Hooks within the function INTERNAL_JUST_DOIT
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_DOIT_HOOK functionname)
	FILE(APPEND "${CMAKE_BINARY_DIR}/Hooks/DoitHooksTemp.cmake" "${functionname}\n")
ENDFUNCTION(ADD_PLUGIN_DOIT_HOOK)

FUNCTION(CALL_PLUGIN_DOIT_HOOKS)
	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DoitHooksTemp.cmake)
		INCLUDE(${CMAKE_BINARY_DIR}/Hooks/DoitHooksTemp.cmake)
	ENDIF()
ENDFUNCTION(CALL_PLUGIN_DOIT_HOOKS)
