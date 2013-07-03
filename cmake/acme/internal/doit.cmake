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

# Must be a macro because of plugins. Implementation as function can lead to undefined behaviour
MACRO(INTERNAL_JUST_DOIT)

	MESSAGE(VERBOSE INTERNAL_JUST_DOIT "Building ${CURRENT_MODULE_NAME}")
	#--------------------------------------------------------------------------
	# Hook for external plugins
	#--------------------------------------------------------------------------

	CALL_PLUGIN_BEFORE_DOIT_HOOKS()


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
    
    INTERNAL_LIST_REMOVE_DUPLICATES(GoogleMock_DEPENDENCIES)
    SET(GoogleMock_DEPENDENCIES ${GoogleMock_DEPENDENCIES} CACHE INTERNAL "")
	
	INTERNAL_SET_MODULE_SOURCE_FILES()						# adds files in the "res/" folders to "${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES"
    
	IF(${WITH_${CURRENT_UPPER_MODULE_NAME}} AND ${${CURRENT_MODULE_NAME}_BUILD_ENABLED})
		# Everything that has to do with collecting things from dependencies
		INTERNAL_COLLECT_LIBRARIES_FOR_LINKING()			# sets the local variable "collected_libs"
		INTERNAL_COLLECT_DEPENDENCY_DIRS()					# sets the local variable "collected_dependency_dirs"
		INTERNAL_COLLECT_INCLUDE_DIRS()						# sets the local variable "include_dirs"
		INTERNAL_COLLECT_RES_FILES()						# sets the local variable "resource_files"
		INTERNAL_ADD_CORRECT_CMAKELIST()					# adds correct "CMakeList.txt" to "${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES"
		INTERNAL_COLLECT_PACKAGE_LINK_DIRECTORIES()			# sets the local variable "external_package_link_directories"

        LINK_DIRECTORIES(${collected_dependency_dirs})

		IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
			# Create actual cmake target and add properties to them
			INTERNAL_ADD_MODULE_TARGET()					# adds the "${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}" to the generated library or executable target
			INTERNAL_SET_TARGET_PROPERTIES()                # sets special target properties.
			INTERNAL_ADD_FLAGS_TO_MODULE_TARGET()			# adds compiler and linker flags as well as debug and release definitions to the library or executable target
			INTERNAL_SET_MODULE_TARGET_PROPERTIES()			# sets the "INCLUDE_DIRECTORIES" and "LINKER_LANGUAGE" property of the library or executable target 
			INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET()	# adds the dependencies within "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" to the library or executable target
			INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET()		# links the libraries within "${${CURRENT_MODULE_NAME}_LIBRARIES}" and "${collected_libs}" to the library or executable target
		ELSE()
			ADD_CUSTOM_TARGET(${CURRENT_MODULE_NAME} SOURCES ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES})
		ENDIF()

		# Test stuff
        INTERNAL_COLLECT_DEPENDENCIES_TEST()				# sets the local variable "collected_dependencies_test"
        INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST()				# sets the local variable "collected_dependency_dirs_test"
		IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
            INTERNAL_SET_GLOBAL_TEST_VARIABLES()
			IF(${CONFIG_BUILD_UNITTESTS})
				IF(NOT ${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})
					
					IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
						IF("${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC") # use an advanced flag to control this behaviour ("DYNAMIC_TEST_LINK_BEHAVIOUR") ?!
							SOURCE_GROUP("Test Files" FILES ${${CURRENT_MODULE_NAME}_TEST_FILES})
							SET(${CURRENT_MODULE_NAME}_TEST_FILES ${${CURRENT_MODULE_NAME}_TEST_FILES} ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES})
						ENDIF()
					ENDIF()			
			
					LINK_DIRECTORIES(${GoogleTest_LIBRARY_DIRS} ${GoogleMock_LIBRARY_DIRS} ${external_package_link_directories})
					ADD_EXECUTABLE(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_TEST_FILES} ${${CURRENT_MODULE_NAME}_TEST_MAIN})
					GET_TARGET_PROPERTY(PREVIOUS_DIRS  ${CURRENT_MODULE_NAME}Test INCLUDE_DIRECTORIES)
					IF ("${PREVIOUS_DIRS}" STREQUAL "PREVIOUS_DIRS-NOTFOUND")
						SET(PREVIOUS_DIRS "")
					ENDIF()
					SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME}Test PROPERTIES INCLUDE_DIRECTORIES "${PREVIOUS_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/include;${include_dirs};${test_path};${GoogleTest_INCLUDE_DIRS};${GoogleMock_INCLUDE_DIRS}")
					INTERNAL_ADD_FLAGS_TO_TEST_TARGET()

					MESSAGE(VERBOSE  "${CURRENT_MODULE_NAME} contains unit tests, building ${CURRENT_MODULE_NAME}Test")

			                MESSAGE(VERBOSE "Test path: ${test_path}")
					IF(NOT EXISTS "${test_path}/main.cpp")
						SET(GoogleMock_LIBRARIES gtest_main gmock_main ${GoogleMock_LIBRARIES})
					ENDIF()


					TARGET_LINK_LIBRARIES(${CURRENT_MODULE_NAME}Test  ${GoogleMock_LIBRARIES} ${GoogleTest_LIBRARIES} ${collected_dependencies_test})
					ADD_DEPENDENCIES(${CURRENT_MODULE_NAME}Test GoogleMock ${CURRENT_MODULE_NAME})
					INSTALL(TARGETS ${CURRENT_MODULE_NAME}Test RUNTIME DESTINATION bin)
				ENDIF()	
			ENDIF()
		ENDIF()
		
	#--------------------------------------------------------------------------
	# Define a grouping for sources in the makefile
	#--------------------------------------------------------------------------	
		
		INTERNAL_GROUP_PUBLIC_HEADERS()			# adds "publicHeaders" to the source group "Public Header Files"
		INTERNAL_GROUP_RES_FILES()				# adds the "${resource_files}" to the source group "Resource files"
		INTERNAL_ADD_SOURCE_GROUPS()			# adds the source groups within "${CURRENT_MODULE_NAME}_SOURCE_GROUPS" to the solution

	#--------------------------------------------------------------------------
	# Copy files to output directory
	
		INTERNAL_COPY_HEADER_FILES()			# copies public header files to the output directory defined by "${CMAKE_HEADER_OUTPUT_DIRECTORY}"
		INTERNAL_COPY_RESOURCE_FILES()			# copies resource files to the output directory defined by "${CMAKE_RESOURCE_OUTPUT_DIRECTORY}"
		INTERNAL_COPY_DOC_FILES()				# copies documentation files to the output directory defined by "${CMAKE_DOC_OUTPUT_DIRECTORY}"	
		INTERNAL_COPY_INSTALL_FILES()			# copies files within "${CURRENT_MODULE_NAME}_INSTALL_FILES" to the output directory defined by "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
		INTERNAL_COPY_TARGETS()				# copies the targets to CMAKE_INSTALL_PREFIX/<bin|lib>
	ENDIF()

	CALL_PLUGIN_AFTER_DOIT_HOOKS()
ENDMACRO(INTERNAL_JUST_DOIT)


MACRO(INTERNAL_COLLECT_LIBRARIES_FOR_LINKING)
MESSAGE (VERBOSE INTERNAL_COLLECT_LIBRARIES_FOR_LINKING "${CURRENT_MODULE_NAME}_LIBRARIES=${${CURRENT_MODULE_NAME}_LIBRARIES}")
	SET(collected_libs "${${CURRENT_MODULE_NAME}_LIBRARIES}")
	INTERNAL_LIST_REMOVE_ITEM(collected_libs "")

		IF(NOT "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" STREQUAL "")
			FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
            MESSAGE(VERBOSE "${CURRENT_MODULE_NAME}_${current_dependency}_ONLY_HEADERS=${${CURRENT_MODULE_NAME}_${current_dependency}_ONLY_HEADERS}")
                IF(NOT ${CURRENT_MODULE_NAME}_${current_dependency}_ONLY_HEADERS)
                    SET(collected_libs ${collected_libs} ${${current_dependency}_LIBRARIES})
                ELSE()
                    MESSAGE(VERBOSE "Not adding libraries of ${current_dependency} to ${CURRENT_MODULE_NAME}, because of ONLY_HEADERS option")
                ENDIF()
			ENDFOREACH()	
			
			LIST(LENGTH collected_libs jd_length)
			IF(jd_length GREATER 1)
				LIST(REVERSE collected_libs)
				LIST(REMOVE_DUPLICATES collected_libs)
				LIST(REVERSE collected_libs)
			ENDIF()
		ENDIF()
	MESSAGE (VERBOSE "The following libs for linking have been collected: ${collected_libs}")
ENDMACRO(INTERNAL_COLLECT_LIBRARIES_FOR_LINKING)


MACRO(INTERNAL_COLLECT_DEPENDENCIES_TEST)
	SET(collected_dependencies_test ${collected_libs})
	MESSAGE(VERBOSE "Initial collected libs in test ${collected_libs}")
	MESSAGE(VERBOSE "List of all dependencies of ${CURRENT_MODULE_NAME}: ${${CURRENT_MODULE_NAME}_DEPENDENCIES}")

	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
		FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
			IF(NOT ${CURRENT_MODULE_NAME}_${current_dependency}_ONLY_HEADERS)
				SET(collected_dependencies_test ${collected_dependencies_test} ${${current_dependency}_LIBRARIES})
			ELSE()
				MESSAGE(VERBOSE "Not adding libraries of ${current_dependency} to ${CURRENT_MODULE_NAME}Test, because of ONLY_HEADERS option")
			ENDIF()
		ENDFOREACH()

		FOREACH (googleMockDep ${GoogleMock_DEPENDENCIES})
			SET(collected_dependencies_test ${collected_dependencies_test} ${${googleMockDep}_LIBRARIES})
		ENDFOREACH()

		MESSAGE(VERBOSE "Collected dependencies for test ${collected_dependencies_test}")

		IF(NOT "${collected_dependencies_test}" STREQUAL "")
			LIST(REVERSE collected_dependencies_test)
			LIST(REMOVE_DUPLICATES collected_dependencies_test)
			LIST(REVERSE collected_dependencies_test)
		ENDIF()
	
		IF(${CONFIG_BUILD_UNITTESTS})
			IF(NOT ${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})
				IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
					IF(NOT "${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC") # use an advanced flag to control this behaviour ("DYNAMIC_TEST_LINK_BEHAVIOUR") ?!
						SET(collected_dependencies_test ${${CURRENT_MODULE_NAME}_LIBRARIES} ${collected_dependencies_test})
					ENDIF()
				ENDIF()
			ENDIF()
		ENDIF()
	ENDIF()

	MESSAGE(VERBOSE "Finally collected libs for linking in test  ${collected_dependencies_test}")

ENDMACRO(INTERNAL_COLLECT_DEPENDENCIES_TEST)


MACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS)
    MESSAGE(VERBOSE INTERNAL_COLLECT_DEPENDENCY_DIRS "${CURRENT_MODULE_NAME}_LIBRARY_DIRS=${${CURRENT_MODULE_NAME}_LIBRARY_DIRS}")
	SET(collected_dependency_dirs "${${CURRENT_MODULE_NAME}_LIBRARY_DIRS}")
	INTERNAL_LIST_REMOVE_ITEM(collected_dependency_dirs "")
	
	IF(NOT "${collected_dependency_dirs}" STREQUAL "")
		LIST(REMOVE_DUPLICATES collected_dependency_dirs)
	ENDIF()
    
    IF(NOT "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" STREQUAL "")
        FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
            SET(collected_dependency_dirs ${collected_dependency_dirs} ${${current_dependency}_LIBRARY_DIRS})
        ENDFOREACH()	
    ENDIF()
    MESSAGE(VERBOSE INTERNAL_COLLECT_DEPENDENCY_DIRS END "${CURRENT_MODULE_NAME}_LIBRARY_DIRS=${${CURRENT_MODULE_NAME}_LIBRARY_DIRS}")
ENDMACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS)


MACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST)
	SET(collected_dependency_dirs_test ${collected_dependency_dirs})

	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")
		IF(${CONFIG_BUILD_UNITTESTS})
			# Add libraries of the module dependencies to the variable (in order to link it to the test-module)
			FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
				SET(collected_dependency_dirs_test "${collected_dependency_dirs_test}" "${${CURRENT_MODULE_NAME}_LIBRARY_DIRS}")
			ENDFOREACH()
		ENDIF()
	ENDIF()
ENDMACRO(INTERNAL_COLLECT_DEPENDENCY_DIRS_TEST)


MACRO(INTERNAL_COLLECT_INCLUDE_DIRS)
	SET(LOCAL_MODULE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/include)
	SET(include_dirs ${${CURRENT_MODULE_NAME}_INCLUDE_DIRS} ${LOCAL_MODULE_INCLUDE_PATH})
	FOREACH(dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
		IF(EXISTS "${PROJECT_SOURCE_DIR}/modules/${dependency}/include")
			SET(include_dirs ${include_dirs} "${PROJECT_SOURCE_DIR}/modules/${dependency}/include")
		ELSEIF(DEFINED ${dependency}_INCLUDE_DIRS)
			SET(include_dirs ${include_dirs} "${${dependency}_INCLUDE_DIRS}")
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
		SET(external_package_link_directories "")
		FOREACH(current_dependency ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
			IF(DEFINED ${current_dependency}_LIBRARY_DIRS)
				SET(external_package_link_directories ${external_package_link_directories} "${${current_dependency}_LIBRARY_DIRS}")
			ENDIF()
		ENDFOREACH()	
ENDMACRO(INTERNAL_COLLECT_PACKAGE_LINK_DIRECTORIES)

MACRO(INTERNAL_SET_TARGET_PROPERTIES)
	IF(NOT "" STREQUAL "${${CURRENT_MODULE_NAME}_OUTPUT_NAME}")
		SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME} PROPERTIES OUTPUT_NAME ${${CURRENT_MODULE_NAME}_OUTPUT_NAME} )
	ENDIF()
ENDMACRO(INTERNAL_SET_TARGET_PROPERTIES)

MACRO(INTERNAL_ADD_MODULE_TARGET)
  MESSAGE(VERBOSE "Adding module ${CURRENT_MODULE_NAME}")
	IF("${CURRENT_MODULE_TYPE}" STREQUAL "STATIC")
		ADD_LIBRARY(${CURRENT_MODULE_NAME}
		STATIC
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
		)
		SET(${CURRENT_MODULE_NAME}_LIBRARIES ${CURRENT_MODULE_NAME} CACHE INTERNAL "")

	ELSEIF("${CURRENT_MODULE_TYPE}" STREQUAL "DYNAMIC")
		ADD_LIBRARY(${CURRENT_MODULE_NAME}
		SHARED
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
		)
		SET(${CURRENT_MODULE_NAME}_LIBRARIES ${CURRENT_MODULE_NAME} CACHE INTERNAL "")	
				
	ELSEIF("${CURRENT_MODULE_TYPE}" STREQUAL "EXE")
		ADD_EXECUTABLE(${CURRENT_MODULE_NAME}
		${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES}
        )

    ENDIF()
ENDMACRO(INTERNAL_ADD_MODULE_TARGET)



MACRO(INTERNAL_ADD_FLAGS_TO_MODULE_TARGET)
	INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS})
	
	INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS})
	INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS})

	INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS})
	INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS})
ENDMACRO(INTERNAL_ADD_FLAGS_TO_MODULE_TARGET)


MACRO(INTERNAL_SET_MODULE_TARGET_PROPERTIES)
	GET_TARGET_PROPERTY(PREVIOUS_DIRS  ${CURRENT_MODULE_NAME} INCLUDE_DIRECTORIES)
	IF ("${PREVIOUS_DIRS}" STREQUAL "PREVIOUS_DIRS-NOTFOUND")
		SET(PREVIOUS_DIRS "")
	ENDIF()
	SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME} PROPERTIES INCLUDE_DIRECTORIES "${include_dirs};${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/include;${PREVIOUS_DIRS}")
	SET_TARGET_PROPERTIES(${CURRENT_MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
ENDMACRO(INTERNAL_SET_MODULE_TARGET_PROPERTIES)


MACRO(INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET)
	IF(NOT "${${CURRENT_MODULE_NAME}_DEPENDENCIES}" STREQUAL "")
		ADD_DEPENDENCIES(${CURRENT_MODULE_NAME} ${${CURRENT_MODULE_NAME}_DEPENDENCIES})
	ENDIF()
ENDMACRO(INTERNAL_ADD_DEPENDENCIES_TO_MODULE_TARGET)


MACRO(INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET)
	IF(NOT "${CURRENT_MODULE_TYPE}" STREQUAL "STATIC")
		TARGET_LINK_LIBRARIES(${CURRENT_MODULE_NAME} ${collected_libs})
	ENDIF()
ENDMACRO(INTERNAL_LINK_LIBRARIES_TO_MODULE_TARGET)


MACRO(INTERNAL_SET_GLOBAL_TEST_VARIABLES)
	IF(NOT "${${CURRENT_MODULE_NAME}_TEST_FILES}" STREQUAL "")	
		SET(GLOBAL_UTILS_MODULES_TESTS ${GLOBAL_UTILS_MODULES_TESTS} ${CURRENT_MODULE_NAME}Test CACHE INTERNAL "stores the module names of all tests modules")

		IF(${CONFIG_BUILD_UNITTESTS})
			IF(${CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE})
				SET(GLOBAL_TEST_INCLUDE_DIRECTORIES ${GLOBAL_TEST_INCLUDE_DIRECTORIES} ${include_dirs} ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/include "${test_path}" CACHE INTERNAL "collect test include directories")					
				SET(GLOBAL_TEST_LIBS ${collected_dependencies_test} ${GLOBAL_TEST_LIBS} )
                SET(GLOBAL_TEST_LIBS ${${CURRENT_MODULE_NAME}_LIBRARIES} ${GLOBAL_TEST_LIBS} CACHE INTERNAL "collect test libs")
                MESSAGE(VERBOSE INTERNAL_SET_GLOBAL_TEST_VARIABLES "GLOBAL_TEST_LIBS=${GLOBAL_TEST_LIBS}")
				
				SET(GLOBAL_TEST_SOURCE ${GLOBAL_TEST_SOURCE} ${${CURRENT_MODULE_NAME}_TEST_FILES} CACHE INTERNAL "collect test source")
				SET(GLOBAL_TEST_LINKER_DIRECTORIES ${GLOBAL_TEST_LINKER_DIRECTORIES} "${GLOBAL_EXTERNAL_LIBRARY_LIBRARIES_DIR}" "${collected_dependency_dirs_test}" "${external_package_link_directories}" CACHE INTERNAL "collect test linker directories")

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

MACRO(INTERNAL_COPY_TARGETS)
	IF(${${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES})
		INSTALL(TARGETS ${CURRENT_MODULE_NAME}
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib)
	ENDIF()
ENDMACRO(INTERNAL_COPY_TARGETS)

MACRO(INTERNAL_COPY_HEADER_FILES)
	# copy header files
	IF(EXISTS "${${CURRENT_MODULE_NAME}_DIR}/include/${CURRENT_MODULE_NAME}")
		INSTALL(DIRECTORY ${${CURRENT_MODULE_NAME}_DIR}/include/${CURRENT_MODULE_NAME} DESTINATION "include" PATTERN ".svn" EXCLUDE)
	ENDIF()
ENDMACRO(INTERNAL_COPY_HEADER_FILES)


MACRO(INTERNAL_COPY_RESOURCE_FILES)
		# copy resource files
		FILE(GLOB_RECURSE jdi_ressource_files "${${CURRENT_MODULE_NAME}_DIR}/res/*.*")
		FILE(GLOB_RECURSE jdi_exclude_files "${${CURRENT_MODULE_NAME}_DIR}/res/.svn/*")
		IF(NOT "${jdi_exclude_files}" STREQUAL "")
			INTERNAL_LIST_REMOVE_ITEM(jdi_ressource_files ${jdi_exclude_files})
		ENDIF()
		INSTALL(FILES ${jdi_ressource_files} DESTINATION "res/${CURRENT_MODULE_NAME}")
        add_custom_command(TARGET ${CURRENT_MODULE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "${CMAKE_RESOURCE_OUTPUT_DIRECTORY}/${CURRENT_MODULE_NAME}/"
                )
        FOREACH(file ${jdi_ressource_files})
            add_custom_command(TARGET ${CURRENT_MODULE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${file}
                "${CMAKE_RESOURCE_OUTPUT_DIRECTORY}/${CURRENT_MODULE_NAME}/"
                )
        ENDFOREACH()
ENDMACRO(INTERNAL_COPY_RESOURCE_FILES)


MACRO(INTERNAL_COPY_DOC_FILES)
	# copy doc files		
	FILE(GLOB_RECURSE jdi_doc_files "${${CURRENT_MODULE_NAME}_DIR}/doc/*.*")
	FILE(GLOB_RECURSE jdi_exclude_files "${${CURRENT_MODULE_NAME}_DIR}/doc/.svn/*")
	IF(NOT "${jdi_exclude_files}" STREQUAL "")
		INTERNAL_LIST_REMOVE_ITEM(jdi_doc_files ${jdi_exclude_files})
	ENDIF()
	INSTALL(FILES ${jdi_doc_files} DESTINATION "doc")
ENDMACRO(INTERNAL_COPY_DOC_FILES)


MACRO(INTERNAL_COPY_INSTALL_FILES)

	IF(NOT "${${CURRENT_MODULE_NAME}_INSTALL_FILES}" STREQUAL "")
        FOREACH(file ${${CURRENT_MODULE_NAME}_INSTALL_FILES})
			SET(subDir "")
			IF (NOT "${${CURRENT_MODULE_NAME}_${file}_INSTALL_DIR}" STREQUAL "")
				SET(subDir "${${CURRENT_MODULE_NAME}_${file}_INSTALL_DIR}/")
				MESSAGE(VERBOSE "installing ${file} to ${subDir}")
				
				add_custom_command(TARGET ${CURRENT_MODULE_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E make_directory
					"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${subDir}"
                )
			ENDIF()

            add_custom_command(TARGET ${CURRENT_MODULE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${file}
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${subDir}"
                )
				
			INSTALL(FILES ${file} DESTINATION "bin/${subDir}")
        ENDFOREACH()
	ENDIF()
ENDMACRO(INTERNAL_COPY_INSTALL_FILES)


MACRO(INTERNAL_ADD_FLAGS_TO_TEST_TARGET)
	INTERNAL_ADD_COMPILER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS} ${${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS})

	INTERNAL_ADD_DEBUG_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS})
	INTERNAL_ADD_RELEASE_LINKER_FLAGS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS})

	INTERNAL_ADD_DEBUG_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS})
	INTERNAL_ADD_RELEASE_DEFINITIONS_TO_TARGET(${CURRENT_MODULE_NAME}Test ${${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS})
ENDMACRO(INTERNAL_ADD_FLAGS_TO_TEST_TARGET)
