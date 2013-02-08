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

SET(CONFIG_CREATE_TEST_COVERAGE	  0	 CACHE BOOL	   "enable the creation of the test coverage report")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

PRINT_DETAILS(STATUS "...requires external tool Lcov")
IF(CONFIG_CREATE_TEST_COVERAGE)
	IF(NOT LCOV_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindLcov.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_CREATE_TEST_COVERAGE)
	ADD_PLUGIN_FINALIZE_HOOK(INTERNAL_CREATE_TEST_COVERAGE)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_CREATE_TEST_COVERAGE)
    IF(CONFIG_CREATE_TEST_COVERAGE)
		IF(NOT "${LCOV_PATH}" STREQUAL "LCOV_PATH-NOTFOUND")
			MESSAGE("")
			MESSAGE(---------------------------------------------------------------------------)
			MESSAGE("Creating test coverage...")

			IF("${TARGET_OS}" STREQUAL "Linux")
				INTERNAL_TEST_COVERAGE_LINUX()
			ELSEIF("${TARGET_OS}" STREQUAL "Windows")
				MESSAGE(STATUS "The feature is currently just available on Linux")
			ELSE()

			ENDIF()
		ENDIF()
	ENDIF()
ENDFUNCTION(INTERNAL_CREATE_TEST_COVERAGE)


FUNCTION(INTERNAL_TEST_COVERAGE_LINUX) 
    SET(settings_correct 1)

	IF("${GLOBAL_UTILS_MODULES_TESTS}" STREQUAL "")
	    MESSAGE(STATUS "WARNING: No test modules have been found.")
	    SET(settings_correct 0)
	    
	ELSE()
        IF("${LCOV_PATH}" STREQUAL "LCOV_PATH-NOTFOUND")
            MESSAGE(STATUS "WARNING: The required tool lcov is not installed. Install \"lcov\" to create the test coverage target")
            SET(settings_correct 0)
        ENDIF()
	
		IF(CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE)			
            MESSAGE(STATUS "WARNING: The cache variable \"CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE\" ist TRUE. To create the target \"Test_Coverage\" DISABLE this cache variable.")
		    SET(settings_correct 0)
	    ENDIF()
			
        IF(NOT CONFIG_BUILD_UNITTESTS)
	    	MESSAGE(STATUS "WARNING: The cache variable \"CONFIG_BUILD_UNITTESTS\" ist FALSE. To create the target \"Test_Coverage\" ENABLE this cache variable.")
		    SET(settings_correct 0)
	    ENDIF()	
	    
	    IF(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	       	MESSAGE(STATUS "WARNING: The cache variable \"CMAKE_BUILD_TYPE\" ist not \"Debug\". To create the target \"Test_Coverage\" set this cache variable to \"Debug\".")
		    SET(settings_correct 0) 
	    ENDIF()
	ENDIF()


	IF("${settings_correct}" STREQUAL "0")
		MESSAGE(STATUS "WARNING: The target \"Test_Coverage\" was not created.")
	ELSE()
		MESSAGE(STATUS "All necessary settings are correct. A Test-Coverage-Target has been created.")
		MESSAGE(STATUS "Use \"make Test_Coverage\" after building and installing the project in the build-directory to generate the test coverage.")
		
        SET(path_to_testexe "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
		SET(path_to_test_coverage "${CMAKE_DOC_OUTPUT_DIRECTORY}/test_coverage")
		SET(path_to_build_dir "${GLOBAL_TOP_LEVEL_BINARY_DIR}")	 
	    
	    SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	    SET(GCC_COVERAGE_LINK_FLAGS    "-fprofile-arcs")

        FOREACH(module_name ${GLOBAL_MODULE_NAMES})
             STRING(TOUPPER ${module_name} upper_module_name)
             IF(${WITH_${upper_module_name}} AND ${${module_name}_BUILD_ENABLED})
                 INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET("${module_name}" "${GCC_COVERAGE_COMPILE_FLAGS}")
                 INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET("${module_name}" "${GCC_COVERAGE_LINK_FLAGS}")
             ENDIF()
        ENDFOREACH()
        
        FOREACH(test_name ${GLOBAL_UTILS_MODULES_TESTS})
                 INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET("${test_name}" "${GCC_COVERAGE_COMPILE_FLAGS}")
                 INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET("${test_name}" "${GCC_COVERAGE_LINK_FLAGS}")
        ENDFOREACH()

        IF(NOT IS_DIRECTORY ${path_to_test_coverage})
            FILE(MAKE_DIRECTORY ${path_to_test_coverage})
        ENDIF()
        
		IF(EXISTS ${ACME_PATH}/plugins/BuildReport/2_test_coverage_template.dox)
			CONFIGURE_FILE(${ACME_PATH}/plugins/BuildReport/2_test_coverage_template.dox "${CMAKE_DOC_OUTPUT_DIRECTORY}/")
        ENDIF()

        ADD_CUSTOM_TARGET(Test_Coverage 
                          COMMENT "Creating target for test coverage..."  
                          VERBATIM)

        ADD_DEPENDENCIES(Test_Coverage ${GLOBAL_UTILS_MODULES_TESTS})

        FOREACH(entry ${GLOBAL_UTILS_MODULES_TESTS})
            ADD_CUSTOM_COMMAND(TARGET Test_Coverage           
                               POST_BUILD 
                               COMMAND lcov
                                   ARGS -d ${path_to_build_dir} --zerocounters
                               COMMAND lcov
                                   ARGS -c -i -d ${path_to_build_dir}/modules -o base_${entry}.info   
                               WORKING_DIRECTORY "${path_to_test_coverage}"  
                               COMMENT "Initializing code coverage counters..."  
                               )
                               
            ADD_CUSTOM_COMMAND(TARGET Test_Coverage                       
                               COMMAND ./${entry}
                               WORKING_DIRECTORY "${path_to_testexe}"
                               COMMENT "Executing test executables..."
                               )
            
            ADD_CUSTOM_COMMAND(TARGET Test_Coverage                   
                               COMMAND lcov
                                   ARGS -c -d ${path_to_build_dir}/modules -o test_${entry}.info
                               COMMAND lcov
                                   ARGS -a base_${entry}.info -a test_${entry}.info -o total_${entry}.info
                               WORKING_DIRECTORY "${path_to_test_coverage}"
                               COMMENT "Generating test coverage file for module ${entry}..."                          
                               )
        ENDFOREACH()
        
        SET(collect_coverage_files "")
        FOREACH(entry ${GLOBAL_UTILS_MODULES_TESTS})
            SET(collect_coverage_files ${collect_coverage_files} -a total_${entry}.info)
        ENDFOREACH()
        SET(collect_coverage_files ${collect_coverage_files} -o total_${PROJECT_NAME}.info)
        
        ADD_CUSTOM_COMMAND(TARGET Test_Coverage
                           POST_BUILD
                           COMMAND lcov        
                               ARGS ${collect_coverage_files}
                           COMMAND genhtml
		                       ARGS -s -l total_${PROJECT_NAME}.info
		                   WORKING_DIRECTORY "${path_to_test_coverage}"
		                   COMMENT "Merging test coverage files of the testmodules and creating the html output of the test coverage..."
	                      )
			
	ENDIF()
ENDFUNCTION(INTERNAL_TEST_COVERAGE_LINUX)




