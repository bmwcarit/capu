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

# Adds curly braces if argument input does not contain any
MACRO(AddBracesIfNeeded output input)
   IF (NOT "${${input}}" MATCHES ".*\\(.*\\).*$")
        SET(${output} "${${input}}()")
   ELSE()
       SET(${output} "${${input}}")
   ENDIF()
ENDMACRO()

#--------------------------------------------------------------------------
# Hooks within the function INTERNAL_FINALIZE
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_FINALIZE_HOOK functionname)
    AddBracesIfNeeded(withBraces functionname)
	FILE(APPEND "${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake" "${withBraces}\n")
ENDFUNCTION(ADD_PLUGIN_FINALIZE_HOOK)

MACRO(CALL_PLUGIN_FINALIZE_HOOKS)
	IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
		INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
	ENDIF()
ENDMACRO(CALL_PLUGIN_FINALIZE_HOOKS)


#--------------------------------------------------------------------------
# Hooks within the function INTERNAL_ADD_DEPENDENCY
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_DEPENDENCY_HOOK functionname)
    AddBracesIfNeeded(withBraces functionname)
	FILE(APPEND "${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake" "${withBraces}\n")
ENDFUNCTION(ADD_PLUGIN_DEPENDENCY_HOOK)

MACRO(CALL_PLUGIN_DEPENDENCY_HOOKS)
	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
		INCLUDE(${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
	ENDIF()
ENDMACRO(CALL_PLUGIN_DEPENDENCY_HOOKS)


#--------------------------------------------------------------------------
# Hooks within the function INTERNAL_JUST_DOIT
#--------------------------------------------------------------------------

FUNCTION(ADD_PLUGIN_BEFORE_DOIT_HOOK functionname)
    AddBracesIfNeeded(withBraces functionname)
	FILE(APPEND "${CMAKE_BINARY_DIR}/Hooks/BeforeDoitHooksTemp.cmake" "${withBraces}\n")
ENDFUNCTION(ADD_PLUGIN_BEFORE_DOIT_HOOK)

MACRO(CALL_PLUGIN_BEFORE_DOIT_HOOKS)
	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/BeforeDoitHooksTemp.cmake)
		INCLUDE(${CMAKE_BINARY_DIR}/Hooks/BeforeDoitHooksTemp.cmake)
	ENDIF()
ENDMACRO(CALL_PLUGIN_BEFORE_DOIT_HOOKS)

FUNCTION(ADD_PLUGIN_AFTER_DOIT_HOOK functionname)
    AddBracesIfNeeded(withBraces functionname)
	FILE(APPEND "${CMAKE_BINARY_DIR}/Hooks/AfterDoitHooksTemp.cmake" "${withBraces}\n")
ENDFUNCTION(ADD_PLUGIN_AFTER_DOIT_HOOK)

MACRO(CALL_PLUGIN_AFTER_DOIT_HOOKS)
	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/AfterDoitHooksTemp.cmake)
		INCLUDE(${CMAKE_BINARY_DIR}/Hooks/AfterDoitHooksTemp.cmake)
 	ENDIF()
ENDMACRO(CALL_PLUGIN_AFTER_DOIT_HOOKS)

FUNCTION(CLEAN_UP_HOOKS)
	MESSAGE(VERBOSE Hooks "Cleaning up hooks")
	IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
		FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/Hooks/FinalizeHooksTemp.cmake)
	ENDIF()

	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
		FILE(REMOVE ${CMAKE_BINARY_DIR}/Hooks/DependencyHooksTemp.cmake)
	ENDIF()

	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/BeforeDoitHooksTemp.cmake)
		FILE(REMOVE ${CMAKE_BINARY_DIR}/Hooks/BeforeDoitHooksTemp.cmake)
	ENDIF()

	IF(EXISTS ${CMAKE_BINARY_DIR}/Hooks/AfterDoitHooksTemp.cmake)
		FILE(REMOVE ${CMAKE_BINARY_DIR}/Hooks/AfterDoitHooksTemp.cmake)
	ENDIF()
ENDFUNCTION(CLEAN_UP_HOOKS)

CLEAN_UP_HOOKS()
