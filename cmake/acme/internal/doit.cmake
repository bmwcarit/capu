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

FUNCTION(INTERNAL_JUST_DOIT)
	
	#--------------------------------------------------------------------------
	# Hook for external plugins
	#--------------------------------------------------------------------------
	
	CALL_PLUGIN_DOIT_HOOKS()
	
	
	#--------------------------------------------------------------------------
	# Set some local variables that are used within INTERNAL_JUST_DOIT
	#--------------------------------------------------------------------------
	
    INTERNAL_ADD_DEFINITION(TARGET_OS=${TARGET_OS})        # Adds the current TARGET_OS to every module (so that the definition can be used within code).
	STRING(TOUPPER ${CURRENT_MODULE_NAME} CURRENT_UPPER_MODULE_NAME)
	SET(CURRENT_MODULE_TYPE	${${CURRENT_MODULE_NAME}_MODULE_TYPE})
	SET(test_path ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/test)
	SET(${CURRENT_MODULE_NAME}_TEST_MAIN "")
	IF(EXISTS "${test_path}/main.cpp")
		SET(${CURRENT_MODULE_NAME}_TEST_MAIN "${test_path}/main.cpp")
	ENDIF()	
	
	IF(${WITH_${CURRENT_UPPER_MODULE_NAME}} AND ${${CURRENT_MODULE_NAME}_BUILD_ENABLED})
		INTERNAL_SET_INTERN_INCLUDE_PATH()					# sets the local variable "intern_include_path"
		INTERNAL_COLLECT_DEPENDENCIES()						# sets the local variable "collected_dependencies"
		INTERNAL_COLLECT_DEPENDENCIES_TEST()				# sets the local variable "collected_dependencies_test"
		INTERNAL_COLLECT_DEPENDENCY_DIRS()					# sets the local variable "collected_dependency_dirs"
		INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST()				# sets the local variable "collected_dependency_dirs_test"
		INTERNAL_COLLECT_INCLUDE_DIRS()						# sets the local variable "include_dirs"
		INTERNAL_COLLECT_RES_FILES()						# sets the local variable "resource_files"
		INTERNAL_ADD_CORRECT_CMAKELIST()					# adds correct "CMakeList.txt" to "${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES"
		INTERNAL_COLLECT_PACKAGE_LINK_DIRECTORIES()			# sets the local variable "external_package_link_directories"
	ENDIF()
	
	#--------------------------------------------------------------------------
	# Set some global variables 
	#--------------------------------------------------------------------------
	
	INTERNAL_SET_GLOBAL_TEST_VARIABLES()
	INTERNAL_SET_MODULE_SOURCE_FILES()						# adds files in the "res/" folders to "${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES"
	INTERNAL_DOIT_REMOVE_DUPLICATES()	                

	
	#--------------------------------------------------------------------------
	# Forward local variables to CMake commands
	#--------------------------------------------------------------------------	
	
	IF(${WITH_${CURRENT_UPPER_MODULE_NAME}} AND ${${CURRENT_MODULE_NAME}_BUILD_ENABLED})
		
		IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
			LINK_DIRECTORIES(${GLOBAL_EXTERNAL_LIBRARY_LIBRARIES_DIR} ${collected_dependency_dirs})
			INTERNAL_ADD_MODULE_TARGET()					# adds the "${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}" to the generated library or executable target
			INTERNAL_ADD_FLAGS_TO_MODULE_TARGET()			# adds compiler and linker flags as well as debug and release definitions to the library or executable target
			INTERNAL_SET_MODULE_TARGET_PROPERTIES()			# sets the "INCLUDE_DIRECTORIES" and "LINKER_LANGUAGE" property of the library or executable target 
			INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET()	# adds the dependencies within "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" to the library or executable target
			INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET()		# links the libraries within "${${CURRENT_MODULE_NAME}_LIBRARIES}" and "${collected_dependencies}" to the library or executable target
		ELSE()
			ADD_CUSTOM_TARGET(${CURRENT_MODULE_NAME} SOURCES ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES})
		ENDIF()

		IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
			IF(${CONFIG_BUILD_UNITTESTS})
				IF(NOT ${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})
					
					IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
						IF("${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC") # use an advanced flag to control this behaviour ("DYNAMIC_TEST_LINK_BEHAVIOUR") ?!
							SOURCE_GROUP("Test Files" FILES ${${CURRENT_MODULE_NAME}_TEST_FILES})
							SET(${CURRENT_MODULE_NAME}_TEST_FILES ${${CURRENT_MODULE_NAME}_TEST_FILES} ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES})
						ENDIF()
					ENDIF()			
			
					LINK_DIRECTORIES(${GoogleTest_LIBRARIES_DIR} ${GoogleMock_LIBRARIES_DIR} ${external_package_link_directories})
					INCLUDE_DIRECTORIES(${intern_include_path} ${include_dirs} ${test_path})
					INCLUDE_DIRECTORIES(SYSTEM ${GoogleTest_INCLUDE_DIRS} ${GoogleMock_INCLUDE_DIRS})
					ADD_EXECUTABLE(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_TEST_FILES} ${${CURRENT_MODULE_NAME}_TEST_MAIN})
					INTERNAL_ADD_FLAGS_TO_TEST_TARGET()

					MESSAGE(STATUS "${CURRENT_MODULE_NAME} contains unit tests, building ${CURRENT_MODULE_NAME}Test")
					TARGET_LINK_LIBRARIES(${CURRENT_MODULE_NAME}Test ${GoogleMock_LIBRARIES} ${GoogleTest_LIBRARIES} ${GoogleMock_PACKAGE_LIBS} ${GoogleTest_PACKAGE_LIBS} ${collected_dependencies_test})
					ADD_DEPENDENCIES(${CURRENT_MODULE_NAME}Test GoogleMock ${CURRENT_MODULE_NAME})
				ENDIF()	
			ENDIF()
		ENDIF()
		
	ENDIF()	
		
		
	#--------------------------------------------------------------------------
	# Define a grouping for sources in the makefile
	#--------------------------------------------------------------------------	
		
	IF(${WITH_${CURRENT_UPPER_MODULE_NAME}} AND ${${CURRENT_MODULE_NAME}_BUILD_ENABLED})
		INTERNAL_GROUP_PUBLIC_HEADERS()			# adds "publicHeaders" to the source group "Public Header Files"
		INTERNAL_GROUP_RES_FILES()				# adds the "${resource_files}" to the source group "Resource files"
		INTERNAL_ADD_SOURCE_GROUPS()			# adds the source groups within "${CURRENT_MODULE_NAME}_SOURCE_GROUPS" to the solution
	ENDIF()
		
		
	#--------------------------------------------------------------------------
	# Copy files to output directory
	#--------------------------------------------------------------------------			
	
	IF(${WITH_${CURRENT_UPPER_MODULE_NAME}} AND ${${CURRENT_MODULE_NAME}_BUILD_ENABLED})
		INTERNAL_COPY_HEADER_FILES()			# copies public header files to the output directory defined by "${CMAKE_HEADER_OUTPUT_DIRECTORY}"
		INTERNAL_COPY_RESOURCE_FILES()			# copies resource files to the output directory defined by "${CMAKE_RESOURCE_OUTPUT_DIRECTORY}"
		INTERNAL_COPY_DOC_FILES()				# copies documentation files to the output directory defined by "${CMAKE_DOC_OUTPUT_DIRECTORY}"	
		INTERNAL_COPY_INSTALL_FILES()			# copies files within "${CURRENT_MODULE_NAME}_INSTALL_FILES" to the output directory defined by "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
	ENDIF()
	
ENDFUNCTION(INTERNAL_JUST_DOIT)


MACRO(INTERNAL_SET_INTERN_INCLUDE_PATH)
	SET(intern_include_path ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/include)
ENDMACRO(INTERNAL_SET_INTERN_INCLUDE_PATH)


MACRO(INTERNAL_COLLECT_DEPENDENCIES)
	SET(collected_dependencies "${${CURRENT_MODULE_NAME}_LIBRARIES}" "${${CURRENT_MODULE_NAME}_PACKAGE_LIBS}")
	LIST(REMOVE_ITEM collected_dependencies "")

	IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
		IF(NOT "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" STREQUAL "")
			FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
				SET(collected_dependencies ${collected_dependencies} ${${current_dependency}_PACKAGE_LIBS})
				#	IF((${${current_dependency}_HAS_SOURCE_FILES}) AND NOT( "${${current_dependency}_MODULE_TYPE}" STREQUAL "EXE"))
				#		SET(collected_dependencies ${collected_dependencies} "${current_dependency}")
				#	ELSEIF(DEFINED ${current_dependency}_LIBRARIES)
				#		SET(collected_dependencies ${collected_dependencies} ${current_dependency})
				#	ENDIF()
			ENDFOREACH()	
			
			LIST(LENGTH collected_dependencies jd_length)
			IF(jd_length GREATER 1)
				LIST(REVERSE collected_dependencies)
				LIST(REMOVE_DUPLICATES collected_dependencies)
				LIST(REVERSE collected_dependencies)
			ENDIF()
		ENDIF()
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_DEPENDENCIES)


MACRO(INTERNAL_COLLECT_DEPENDENCIES_TEST)
	SET(collected_dependencies_test ${collected_dependencies})
	
	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
		FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
			IF(${${current_dependency}_HAS_SOURCE_FILES})
				SET(collected_dependencies_test ${collected_dependencies_test} ${current_dependency})
			ENDIF()
		ENDFOREACH()
		
		IF(NOT "${collected_dependencies_test}" STREQUAL "")
			LIST(REVERSE collected_dependencies_test)
			LIST(REMOVE_DUPLICATES collected_dependencies_test)
			LIST(REVERSE collected_dependencies_test)
		ENDIF()
	
		IF(${CONFIG_BUILD_UNITTESTS})
			# Add libraries of the module dependencies to the variable (in order to link it to the test-module)
			FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
				SET(collected_dependencies_test "${collected_dependencies_test}" ${${current_dependency}_LIBRARIES} ${${current_dependency}_PACKAGE_LIBS})
			ENDFOREACH()
			
			IF(NOT "${collected_dependencies_test}" STREQUAL "")
				LIST(REVERSE collected_dependencies_test)
				LIST(REMOVE_DUPLICATES collected_dependencies_test)
				LIST(REVERSE collected_dependencies_test)
			ENDIF()		
				
			IF(NOT ${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})			
				IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
					IF(NOT "${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC") # use an advanced flag to control this behaviour ("DYNAMIC_TEST_LINK_BEHAVIOUR") ?!
						SET(collected_dependencies_test ${CURRENT_MODULE_NAME} ${collected_dependencies_test})
					ENDIF()
				ENDIF()
				
				IF(NOT EXISTS "${test_path}/main.cpp")
					SET(collected_dependencies_test "${collected_dependencies_test}" gtest_main gmock_main)
				ENDIF()		
			ENDIF()
		ENDIF()
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_DEPENDENCIES_TEST)


MACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS)
	SET(collected_dependency_dirs "${${CURRENT_MODULE_NAME}_PACKAGE_LIB_DIRS}")
	LIST(REMOVE_ITEM collected_dependency_dirs "")
	
	IF(NOT "${collected_dependency_dirs}" STREQUAL "")
		LIST(REMOVE_DUPLICATES collected_dependency_dirs)
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS)


MACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST)
	SET(collected_dependency_dirs_test ${collected_dependency_dirs})
	
	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
		IF(${CONFIG_BUILD_UNITTESTS})
			# Add libraries of the module dependencies to the variable (in order to link it to the test-module)
			FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
				SET(collected_dependency_dirs_test "${collected_dependency_dirs_test}" "${${CURRENT_MODULE_NAME}_PACKAGE_LIB_DIRS}")
			ENDFOREACH()
		ENDIF()
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST)


MACRO(INTERNAL_COLLECT_INCLUDE_DIRS)
	SET(include_dirs ${${CURRENT_MODULE_NAME}_INCLUDE_DIRS})
	FOREACH(dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
		IF(EXISTS "${PROJECT_SOURCE_DIR}/modules/${dependency}/include")
			SET(include_dirs ${include_dirs} "${PROJECT_SOURCE_DIR}/modules/${dependency}/include")
		ELSEIF(DEFINED ${dependency}_INCLUDE_DIRS)
			SET(include_dirs ${include_dirs} "${${dependency}_INCLUDE_DIRS}")
			SET(GLOBAL_EXTERN_INCLUDE_DIRS ${GLOBAL_EXTERN_INCLUDE_DIRS} "${${dependency}_INCLUDE_DIRS}" CACHE INTERNAL "collect extern include dirs")
		ELSE()
			MESSAGE(FATAL_ERROR "Added unknown dependency ${dependency}")
		ENDIF()
	ENDFOREACH()
ENDMACRO(INTERNAL_COLLECT_INCLUDE_DIRS)


MACRO(INTERNAL_COLLECT_RES_FILES)
	# collect the res files
	IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/res")
		FILE(GLOB_RECURSE resource_files "${CURRENT_MODULE_NAME}/res/*.*") # TODO how to keep the file directory structure?
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_RES_FILES)


MACRO(INTERNAL_ADD_CORRECT_CMAKELIST)
	# add cmake file from current directory to target for display in visual studio projects
	SET(${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/CMakeLists.txt" CACHE INTERNAL "")
	SOURCE_GROUP("_" FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
ENDMACRO(INTERNAL_ADD_CORRECT_CMAKELIST)


MACRO(INTERNAL_COLLECT_PACKAGE_LINK_DIRECTORIES)
	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
		SET(external_package_link_directories "")
		FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
			IF(DEFINED ${current_dependency}_LIBRARIES_DIR)
				SET(external_package_link_directories ${external_package_link_directories} "${${current_dependency}_LIBRARIES_DIR}")
			ENDIF()
		ENDFOREACH()	
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_PACKAGE_LINK_DIRECTORIES)


MACRO(INTERNAL_ADD_MODULE_TARGET)
	IF("${CURRENT_MODULE_TYPE}" STREQUAL "STATIC")
		#SET(GLOBAL_UTILS_MODULES_STATIC ${GLOBAL_UTILS_MODULES_STATIC} ${CURRENT_MODULE_NAME} CACHE INTERNAL "stores the module names of all static modules")
		ADD_LIBRARY(${CURRENT_MODULE_NAME}
		STATIC
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
		)
		SET(GLOBAL_EXTERNAL_LIBRARY_LIBRARIES ${GLOBAL_EXTERNAL_LIBRARY_LIBRARIES} ${CURRENT_MODULE_NAME}  CACHE INTERNAL "global list of all variables which indicate the libraries of external libraries")
			
	ELSEIF("${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC")
		#SET(GLOBAL_UTILS_MODULES_DYNAMIC ${GLOBAL_UTILS_MODULES_DYNAMIC} ${CURRENT_MODULE_NAME} CACHE INTERNAL "stores the module names of all dynamic modules")
		ADD_LIBRARY(${CURRENT_MODULE_NAME}
		SHARED
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
		)
				
	ELSEIF("${CURRENT_MODULE_TYPE}" STREQUAL "EXE")
		#SET(GLOBAL_UTILS_MODULES_EXE ${GLOBAL_UTILS_MODULES_EXE} ${CURRENT_MODULE_NAME} CACHE INTERNAL "stores the module names of all exe modules")
		ADD_EXECUTABLE(${CURRENT_MODULE_NAME}
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
        )
		
    ENDIF()		
ENDMACRO(INTERNAL_ADD_MODULE_TARGET)



MACRO(INTERNAL_ADD_FLAGS_TO_MODULE_TARGET)
	INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS})
	#INTERNAL_ADD_RELEASE_COMPILER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS})
	
	INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS})
	INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS})

	INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS})
	INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS})
ENDMACRO(INTERNAL_ADD_FLAGS_TO_MODULE_TARGET)


MACRO(INTERNAL_SET_MODULE_TARGET_PROPERTIES)
	SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME} PROPERTIES INCLUDE_DIRECTORIES "${include_dirs};${intern_include_path}")
	SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
ENDMACRO(INTERNAL_SET_MODULE_TARGET_PROPERTIES)


MACRO(INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET)
	IF(NOT "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" STREQUAL "")
		ADD_DEPENDENCIES(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
	ENDIF()
ENDMACRO(INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET)


MACRO(INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET)
	IF(NOT "${${CURRENT_MODULE_NAME}_LIBRARIES}" STREQUAL "" AND NOT "${CURRENT_MODULE_TYPE}" STREQUAL "STATIC")
		TARGET_LINK_LIBRARIES(${CURRENT_MODULE_NAME} ${collected_dependencies} ${${CURRENT_MODULE_NAME}_LIBRARIES})
	ENDIF()
ENDMACRO(INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET)


MACRO(INTERNAL_SET_GLOBAL_TEST_VARIABLES)
	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")	
		SET(GLOBAL_UTILS_MODULES_TESTS ${GLOBAL_UTILS_MODULES_TESTS} ${CURRENT_MODULE_NAME}Test CACHE INTERNAL "stores the module names of all tests modules")

		IF(${CONFIG_BUILD_UNITTESTS})
			IF(${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})
				SET(GLOBAL_TEST_INCLUDE_DIRECTORIES ${GLOBAL_TEST_INCLUDE_DIRECTORIES} ${include_dirs} ${intern_include_path} "${test_path}" CACHE INTERNAL "collect test include directories")					
				SET(GLOBAL_TEST_LIBS ${collected_dependencies_test} ${GLOBAL_TEST_LIBS} CACHE INTERNAL "collect test libs")
				
				IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
					SET(GLOBAL_TEST_LIBS ${CURRENT_MODULE_NAME} ${GLOBAL_TEST_LIBS} CACHE INTERNAL "collect test libs")
				ENDIF()
				
				SET(GLOBAL_TEST_SOURCE ${GLOBAL_TEST_SOURCE} ${${CURRENT_MODULE_NAME}_TEST_FILES} CACHE INTERNAL "collect test source")
				SET(GLOBAL_TEST_LINKER_DIRECTORIES ${GLOBAL_TEST_LINKER_DIRECTORIES} "${collected_dependency_dirs_test}" "${external_package_link_directories}" CACHE INTERNAL "collect test linker directories")

				IF(NOT "${GLOBAL_TEST_LINKER_DIRECTORIES}" STREQUAL "")
					LIST(REMOVE_DUPLICATES GLOBAL_TEST_LINKER_DIRECTORIES)
				ENDIF()

				SET(GLOBAL_TEST_DEBUG_COMPILER_FLAGS ${GLOBAL_TEST_DEBUG_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS} CACHE INTERNAL "collect test compiler flags")
				SET(GLOBAL_TEST_RELEASE_COMPILER_FLAGS ${GLOBAL_TEST_RELEASE_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS} CACHE INTERNAL "collect test compiler flags")
				
				SET(GLOBAL_TEST_DEBUG_LINKER_FLAGS ${GLOBAL_TEST_DEBUG_LINKER_FLAGS} ${${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS} CACHE INTERNAL "collect test linker flags")
				SET(GLOBAL_TEST_RELEASE_LINKER_FLAGS ${GLOBAL_TEST_RELEASE_LINKER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS} CACHE INTERNAL "collect test linker flags")
				
				SET(GLOBAL_TEST_DEBUG_DEFINITIONS ${GLOBAL_TEST_DEBUG_DEFINITIONS} ${${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS} CACHE INTERNAL "collect test definitions")
				SET(GLOBAL_TEST_RELEASE_DEFINITIONS ${GLOBAL_TEST_RELEASE_DEFINITIONS} ${${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS} CACHE INTERNAL "collect test definitions")
			ENDIF()
		ENDIF()
	ENDIF()
ENDMACRO(INTERNAL_SET_GLOBAL_TEST_VARIABLES)


MACRO(INTERNAL_SET_MODULE_SOURCE_FILES)
	FOREACH(file ${resource_files})
		SET(${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES} ${file} CACHE INTERNAL "")
		# SET(${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES 1 CACHE INTERNAL "")
	ENDFOREACH()
ENDMACRO(INTERNAL_SET_MODULE_SOURCE_FILES)


MACRO(INTERNAL_GROUP_PUBLIC_HEADERS)
	FILE(GLOB publicHeaders "${${CURRENT_MODULE_NAME}_DIR}/include/${CURRENT_MODULE_NAME}/*.h")
	SOURCE_GROUP("Public Header Files" FILES  ${publicHeaders})
ENDMACRO(INTERNAL_GROUP_PUBLIC_HEADERS)


MACRO(INTERNAL_GROUP_RES_FILES)
	IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/res")
		SOURCE_GROUP("Resource files" FILES ${resource_files})
		SET_SOURCE_FILES_PROPERTIES(${resource_files} PROPERTIES HEADER_FILE_ONLY 1)
	ENDIF()
ENDMACRO(INTERNAL_GROUP_RES_FILES)


MACRO(INTERNAL_ADD_SOURCE_GROUPS)
		# Add source groups to solution
		IF(NOT "${${CURRENT_MODULE_NAME}_SOURCE_GROUPS}" STREQUAL "")
			FOREACH(entry ${${CURRENT_MODULE_NAME}_SOURCE_GROUPS})
				# adjustment for windows to get nested source groups in visual studio
				IF("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
					STRING( REGEX REPLACE "/" "\\\\" result ${entry} )
				ELSE()
					SET(result ${entry})
				ENDIF()
				SOURCE_GROUP("${result}" FILES ${${entry}_${CURRENT_MODULE_NAME}})
			ENDFOREACH()
		ENDIF()
ENDMACRO(INTERNAL_ADD_SOURCE_GROUPS)


MACRO(INTERNAL_COPY_HEADER_FILES)
	# copy header files
	IF(EXISTS "${${CURRENT_MODULE_NAME}_DIR}/include/${CURRENT_MODULE_NAME}")
		INSTALL(DIRECTORY ${${CURRENT_MODULE_NAME}_DIR}/include/${CURRENT_MODULE_NAME} DESTINATION "${CMAKE_HEADER_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}" PATTERN ".svn" EXCLUDE)
	ENDIF()
ENDMACRO(INTERNAL_COPY_HEADER_FILES)


MACRO(INTERNAL_COPY_RESOURCE_FILES)
		# copy resource files
		FILE(GLOB_RECURSE jdi_ressource_files "${${CURRENT_MODULE_NAME}_DIR}/res/*.*")
		FILE(GLOB_RECURSE jdi_exclude_files "${${CURRENT_MODULE_NAME}_DIR}/res/.svn/*")
		IF(NOT "${jdi_exclude_files}" STREQUAL "")
			LIST(REMOVE_ITEM jdi_ressource_files ${jdi_exclude_files})
		ENDIF()
		INSTALL(FILES ${jdi_ressource_files} DESTINATION "${CMAKE_RESOURCE_OUTPUT_DIRECTORY}/${CURRENT_MODULE_NAME}")
ENDMACRO(INTERNAL_COPY_RESOURCE_FILES)


MACRO(INTERNAL_COPY_DOC_FILES)
	# copy doc files		
	FILE(GLOB_RECURSE jdi_doc_files "${${CURRENT_MODULE_NAME}_DIR}/doc/*.*")
	FILE(GLOB_RECURSE jdi_exclude_files "${${CURRENT_MODULE_NAME}_DIR}/doc/.svn/*")
	IF(NOT "${jdi_exclude_files}" STREQUAL "")
		LIST(REMOVE_ITEM jdi_doc_files ${jdi_exclude_files})
	ENDIF()
	INSTALL(FILES ${jdi_doc_files} DESTINATION "${CMAKE_DOC_OUTPUT_DIRECTORY}/${CURRENT_MODULE_NAME}")
ENDMACRO(INTERNAL_COPY_DOC_FILES)


MACRO(INTERNAL_COPY_INSTALL_FILES)
	IF(NOT "${${CURRENT_MODULE_NAME}_INSTALL_FILES}" STREQUAL "")
		INSTALL(FILES ${${CURRENT_MODULE_NAME}_INSTALL_FILES} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/\${BUILD_TYPE}")
	ENDIF()
ENDMACRO(INTERNAL_COPY_INSTALL_FILES)


MACRO(INTERNAL_ADD_FLAGS_TO_TEST_TARGET)
	INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS})

	INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS})
	INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS})

	INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS})
	INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS})
ENDMACRO(INTERNAL_ADD_FLAGS_TO_TEST_TARGET)


MACRO(INTERNAL_DOIT_REMOVE_DUPLICATES)
    LIST(LENGTH GoogleMock_PACKAGE_LIBS ListLength)
    IF(NOT ${ListLength} EQUAL 0)
      LIST(REMOVE_DUPLICATES GoogleMock_PACKAGE_LIBS)
      SET(GoogleMock_PACKAGE_LIBS  ${GoogleMock_PACKAGE_LIBS}  CACHE INTERNAL "")
    ENDIF()
ENDMACRO(INTERNAL_DOIT_REMOVE_DUPLICATES)
