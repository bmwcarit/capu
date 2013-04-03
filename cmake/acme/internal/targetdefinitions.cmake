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

FUNCTION(INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET acf_target)                      
	INTERNAL_LIST_TO_STRING("${ARGN}" ARGN)         # CMake uses semicolon-separated lists almost everywhere except in variables or properties 
	                                                # whose names contain "FLAGS" (which predate the semicolon list convention). 
	                                                # There space-separated lists are used.
	GET_TARGET_PROPERTY(current_compile_flags ${acf_target} COMPILE_FLAGS)
	IF(current_compile_flags STREQUAL "current_compile_flags-NOTFOUND")
		SET(current_compile_flags "") 
	ELSE()
		SET(current_compile_flags "${current_compile_flags} ") # Seperate new compiler flags from the existing ones
	ENDIF()
	SET(current_compile_flags "${current_compile_flags}${ARGN}" )
	
MESSAGE(VERBOSE "The target ${acf_target} now has the following compiler flags: ${current_compile_flags}")

	SET_TARGET_PROPERTIES(${acf_target} PROPERTIES COMPILE_FLAGS "${current_compile_flags}" )
ENDFUNCTION(INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET)


FUNCTION(INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET alf_target)
    INTERNAL_LIST_TO_STRING("${ARGN}" ARGN)         # CMake uses semicolon-separated lists almost everywhere except in variables or properties 
	                                                # whose names contain "FLAGS" (which predate the semicolon list convention). 
	                                                # There space-separated lists are used.
    GET_TARGET_PROPERTY(current_debug_flags ${alf_target} LINK_FLAGS_DEBUG)
    IF(current_debug_flags STREQUAL "current_debug_flags-NOTFOUND")
		SET(current_debug_flags "") 
	ELSE()
		SET(current_debug_flags "${current_debug_flags} ") # Seperate new debug linker flags from the existing ones
	ENDIF()
	SET(current_debug_flags "${current_debug_flags}${ARGN}" )
    
	SET_TARGET_PROPERTIES(${alf_target} PROPERTIES LINK_FLAGS_DEBUG "${current_debug_flags}")
ENDFUNCTION(INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET)


FUNCTION(INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET alf_target)
    INTERNAL_LIST_TO_STRING("${ARGN}" ARGN)         # CMake uses semicolon-separated lists almost everywhere except in variables or properties 
	                                                # whose names contain "FLAGS" (which predate the semicolon list convention). 
	                                                # There space-separated lists are used.
    GET_TARGET_PROPERTY(current_release_flags ${alf_target} LINK_FLAGS_RELEASE)
    IF(current_release_flags STREQUAL "current_release_flags-NOTFOUND")
		SET(current_release_flags "") 
	ELSE()
		SET(current_release_flags "${current_release_flags} ") # Seperate new release linker flags from the existing ones
	ENDIF()
	SET(current_release_flags "${current_release_flags}${ARGN}" )

	SET_TARGET_PROPERTIES(${alf_target} PROPERTIES LINK_FLAGS_RELEASE "${current_release_flags}")
ENDFUNCTION(INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET)


FUNCTION(INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET adt_target )  # Compile definitions are set with semicolon-separated list
    GET_TARGET_PROPERTY(current_debug_definitions ${adt_target} COMPILE_DEFINITIONS_DEBUG)
    
    IF(current_debug_definitions STREQUAL "current_debug_definitions-NOTFOUND")
		SET(current_debug_definitions "")  
	ENDIF()
    SET(new_debug_definitions ${current_debug_definitions} ${ARGN})
    SET_TARGET_PROPERTIES(${adt_target} PROPERTIES COMPILE_DEFINITIONS_DEBUG "${new_debug_definitions}")
ENDFUNCTION(INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET)


FUNCTION(INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET adt_target ) # Release definitions are set with semicolon-separated list
    GET_TARGET_PROPERTY(current_release_definitions ${adt_target} COMPILE_DEFINITIONS_RELEASE)
    
    IF(current_release_definitions STREQUAL "current_release_definitions-NOTFOUND")
		SET(current_release_definitions "")  
	ENDIF()
    SET(new_release_definitions ${current_release_definitions} ${ARGN})
    SET_TARGET_PROPERTIES(${adt_target} PROPERTIES COMPILE_DEFINITIONS_RELEASE "${new_release_definitions}")
ENDFUNCTION(INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET)
