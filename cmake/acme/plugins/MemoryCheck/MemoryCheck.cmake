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
# Plugin API
#--------------------------------------------------------------------------

SET(CONFIG_SEARCH_MEMORY_ERRORS   0  CACHE BOOL     "sets memcheck flags and creates the target Memory_Check")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

MESSAGE(VERBOSE "...requires external tool Valgrind")
IF(CONFIG_SEARCH_MEMORY_ERRORS)
	IF(NOT VALGRIND_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindValgrind.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_SEARCH_MEMORY_ERRORS)
	ADD_PLUGIN_FINALIZE_HOOK(INTERNAL_SEARCH_MEMORY_ERRORS)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_SEARCH_MEMORY_ERRORS)
    IF(CONFIG_SEARCH_MEMORY_ERRORS)
		IF(NOT "${VALGRIND_PATH}" STREQUAL "VALGRIND_PATH-NOTFOUND")
			MESSAGE(VERBOSE "Searching for memory errors...")

			IF("${TARGET_OS}" STREQUAL "Linux")
				INTERNAL_MEMORY_ERRORS_LINUX()
						                			                
			ELSEIF("${TARGET_OS}" STREQUAL "Windows")
				MESSAGE(WARNING "The feature is currently just available on Linux")
			ELSE()
		
			ENDIF()
		ENDIF()
    ENDIF()
ENDFUNCTION(INTERNAL_SEARCH_MEMORY_ERRORS)


FUNCTION(INTERNAL_MEMORY_ERRORS_LINUX)
    SET(settings_correct 1)
    SET(memcheck_compile_flags "-g" "-O1")    
    SET(path_to_executable "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	IF("${VALGRIND_PATH}" STREQUAL "VALGRIND_PATH-NOTFOUND")
		MESSAGE(WARNING "The required tool valgrind is not installed. Install \"valgrind\" to create the memory check target")
		SET(settings_correct 0)
	ENDIF()
	
    IF("${GLOBAL_UTILS_MODULES_TESTS}" STREQUAL "")
	    MESSAGE(WARNING "No test modules have been found.")
	    SET(settings_correct 0)
    ENDIF()
    
    IF(NOT CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE)
        MESSAGE(WARNING "The cache variable \"CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE\" ist FALSE. To create the target \"Memory_Check\" ENABLE this cache variable.")
        SET(settings_correct 0)
    ENDIF()
    
    IF(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	    MESSAGE(WARNING "The cache variable \"CMAKE_BUILD_TYPE\" ist not \"Debug\". To create the target \"Memory_Check\" set this cache variable to \"Debug\".")
	    SET(settings_correct 0) 
	ENDIF()

    IF("${settings_correct}" STREQUAL "0")
		MESSAGE(WARNING "The target \"Memory_Check\" was not created.")
	
	ELSE()
        MESSAGE(VERBOSE  "All necessary settings are correct. A Memory-Check-Target has been created.")
        MESSAGE(STATUS "Valgrind: Use \"make Memory_Check\" after building and installing the project in the build-directory to generate the memory leak detection.")
    
        INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET("Test" "${memcheck_compile_flags}")
        
        ADD_CUSTOM_TARGET(Memory_Check 
                          COMMENT "Creating target that searches for memory errors..."  
                          VERBATIM)
    
        ADD_DEPENDENCIES(Memory_Check "Test")
    
        ADD_CUSTOM_COMMAND(TARGET  Memory_Check           
                           POST_BUILD 
                           COMMAND valgrind
                           ARGS --leak-check=yes ./Test
                           WORKING_DIRECTORY "${path_to_executable}"  
                           COMMENT "Executing test executable to check for memory leaks..."  
                           )                  
    ENDIF()                           
ENDFUNCTION(INTERNAL_MEMORY_ERRORS_LINUX)

