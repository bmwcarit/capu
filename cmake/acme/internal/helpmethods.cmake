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

FUNCTION(INTERNAL_ADD_FILES_TO_SOURCE_GROUP source_group_name list_of_files)
	SET(source_group_exists 0)
	FOREACH(entry ${${CURRENT_MODULE_NAME}_SOURCE_GROUPS})
		IF("${entry}" STREQUAL "${source_group_name}")
			SET(source_group_exists 1)
		ENDIF()
	ENDFOREACH()
	
	IF(NOT ${source_group_exists})
		SET(${CURRENT_MODULE_NAME}_SOURCE_GROUPS ${${CURRENT_MODULE_NAME}_SOURCE_GROUPS} "${source_group_name}" CACHE INTERNAL "")
	ENDIF()
	SET(${source_group_name}_${CURRENT_MODULE_NAME} ${${source_group_name}_${CURRENT_MODULE_NAME}} "${list_of_files}" CACHE INTERNAL "")

	LIST(REMOVE_DUPLICATES ${source_group_name}_${CURRENT_MODULE_NAME})
	SET(${source_group_name}_${CURRENT_MODULE_NAME} 	${${source_group_name}_${CURRENT_MODULE_NAME}} CACHE INTERNAL "")
ENDFUNCTION(INTERNAL_ADD_FILES_TO_SOURCE_GROUP)

FUNCTION(message)
	LIST(GET ARGV 0 LOGLEVEL)
	LIST(REMOVE_AT ARGV 0)
	INTERNAL_LIST_TO_STRING("${ARGV}" MSG)
	IF(LOGLEVEL STREQUAL WARNING)
		_MESSAGE("WARNING: ${MSG}") # remove stack trace from output
	ELSEIF(LOGLEVEL STREQUAL VERBOSE)
		IF(CONFIG_VERBOSE)
			_MESSAGE("${MSG}") 
		ENDIF()
	ELSE()
		_MESSAGE(${LOGLEVEL} " ${MSG}")
	ENDIF()

ENDFUNCTION()


# Split arguments passed to a function into several lists separated by
# specified identifiers that do not have an associated list e.g.:
#
# SET(arguments
#   hello world
#   LIST3 foo bar
#   LIST1 fuz baz
#   )
# ARGUMENT_SPLITTER("${arguments}" "LIST1 LIST2 LIST3" ARG)
#
# results in 8 distinct variables:
#  * ARG_DEFAULT_FOUND: 1
#  * ARG_DEFAULT: hello;world
#  * ARG_LIST1_FOUND: 1
#  * ARG_LIST1: fuz;baz
#  * ARG_LIST2_FOUND: 0
#  * ARG_LIST2:
#  * ARG_LIST3_FOUND: 1
#  * ARG_LIST3: foo;bar
MACRO(INTERNAL_ARGUMENT_SPLITTER argInput argKeywordList argPrefix)

	SET(inputTokenList "DEFAULT" "${argInput}")
	SET(keywordList "DEFAULT ${argKeywordList}")
	SET(splitter_prefix ${argPrefix})

	# convert provided inputTokenList to list
	# SEPARATE_ARGUMENTS(inputTokenList)
	SEPARATE_ARGUMENTS(keywordList)

	# initialize all out variables to default values
	FOREACH(keyword ${keywordList})
		SET(${splitter_prefix}_${keyword}_FOUND 0)
		SET(${splitter_prefix}_${keyword} "")
	ENDFOREACH(keyword)
	
	# iterate all tokens of provided input
	FOREACH(token ${inputTokenList})
		# check if current token is a keyword
		SET(tokenIsKeyword 0)
		FOREACH(keyword ${keywordList})
			IF ("${keyword}" STREQUAL "${token}")
				SET(tokenIsKeyword 1)
				SET(lastKeyword ${keyword})
			ENDIF ("${keyword}" STREQUAL "${token}")
		ENDFOREACH(keyword)

		IF (${tokenIsKeyword})
			# if current token is keyword, set found variable to true
			SET(${splitter_prefix}_${token}_FOUND 1)
		ELSE (${tokenIsKeyword})
			# if current token is not keyword, append token to variable of last found keyword
			SET(${splitter_prefix}_${lastKeyword} ${${splitter_prefix}_${lastKeyword}} ${token})
			#message(${${splitter_prefix}_${lastKeyword}}!!)
		ENDIF (${tokenIsKeyword})
	ENDFOREACH(token)
	
ENDMACRO(INTERNAL_ARGUMENT_SPLITTER)


MACRO(INTERNAL_LIST_TO_STRING ilts_input ilts_output)
	STRING(REGEX REPLACE ";" " " ${ilts_output} "${ilts_input}")
ENDMACRO(INTERNAL_LIST_TO_STRING)


MACRO(INTERNAL_SPLIT_PATH_AND_FILE file_with_path path filename)
	GET_FILENAME_COMPONENT(${path} ${file_with_path} PATH)
	IF(NOT "${${path}}" STREQUAL "")
		SET(${path} "${${path}}/")
	ENDIF()
	GET_FILENAME_COMPONENT(${filename} ${file_with_path} NAME)
ENDMACRO(INTERNAL_SPLIT_PATH_AND_FILE)


MACRO(INTERNAL_GET_PATH_TO_FILES gsp_class)						
	INTERNAL_ARGUMENT_SPLITTER("${ARGN}" "PREFIX" GSP)
	INTERNAL_SPLIT_PATH_AND_FILE(${gsp_class} GSP_PATH_TO_FILE GSP_FILENAME)

	STRING(TOUPPER ${GSP_FILENAME} gsp_varname)
	IF(NOT "${GSP_PREFIX}" STREQUAL "")
		SET(gsp_varname ${GSP_PREFIX})
	ENDIF()
     
	SET(gsp_all_classes ${gsp_class} ${GSP_DEFAULT_ARGS})

	SET(${gsp_varname}_PUBLIC_HEADER)
	SET(${gsp_varname}_INTERN_HEADER)	# collects the intern include files located at "${CMAKE_CURRENT_SOURCE_DIR}/include" for a specific module
	SET(${gsp_varname}_SOURCE_FILES)	# collects the source-files (.cpp or .c) locatet at "${CMAKE_CURRENT_SOURCE_DIR}/src" for a specific module	

	FOREACH(gsp_current_class ${gsp_all_classes})
		INTERNAL_GET_PUBLIC_INCLUDE_PATH( gsp_public_include_path )
		INTERNAL_GET_INTERN_INCLUDE_PATH( gsp_intern_include_path )				
		
		FOREACH(gsp_current_header_type ${GLOBAL_HEADER_FILE_EXTENSIONS})
			# PUBLIC HEADERS:
			SET(gsp_public_include_file "${gsp_public_include_path}/${gsp_current_class}.${gsp_current_header_type}" )
			
			IF(EXISTS ${gsp_public_include_file})
				SET(${gsp_varname}_PUBLIC_HEADER ${${gsp_varname}_PUBLIC_HEADER} ${gsp_public_include_file})
			ENDIF()
			
			# PRIVATE HEADERS:
			SET(gsp_intern_include_file "${gsp_intern_include_path}/${gsp_current_class}.${gsp_current_header_type}" )
			
			IF(EXISTS ${gsp_intern_include_file})
				SET(${gsp_varname}_INTERN_HEADER ${${gsp_varname}_INTERN_HEADER} ${gsp_intern_include_file})
			ENDIF()
			
		ENDFOREACH()
           
		FOREACH(gsp_current_source_type ${GLOBAL_SOURCE_FILE_EXTENSIONS})
		
			INTERNAL_GET_SOURCE_PATH(gsp_source_path)
			SET(gsp_source_file "${gsp_source_path}/${gsp_current_class}.${gsp_current_source_type}" )
			IF(EXISTS ${gsp_source_file})
				SET(${gsp_varname}_SOURCE_FILES ${${gsp_varname}_SOURCE_FILES} ${gsp_source_file})
			ENDIF()
		ENDFOREACH()
		
		INTERNAL_GET_TEST_PATH( gsp_test_path )
		SET(gsp_test_header "${gsp_test_path}/${gsp_current_class}Test.h")
		SET(gsp_test_source "${gsp_test_path}/${gsp_current_class}Test.cpp")
                           					   
		IF(EXISTS ${gsp_test_header})
			SET(${gsp_varname}_TEST_FILES ${${gsp_varname}_TEST_FILES} ${gsp_test_header})  # collects the test files located at "${CMAKE_CURRENT_SOURCE_DIR}/test"
		ENDIF()
                               
		IF(EXISTS ${gsp_test_source})
			SET(${gsp_varname}_TEST_FILES ${${gsp_varname}_TEST_FILES} ${gsp_test_source})  # collects the test files located at "${CMAKE_CURRENT_SOURCE_DIR}/test"
		ENDIF()
                               
	ENDFOREACH()
ENDMACRO(INTERNAL_GET_PATH_TO_FILES)


MACRO(INTERNAL_GET_PUBLIC_INCLUDE_PATH var)
	INTERNAL_GET_MODULE_NAME(gpip_module_name)
	INTERNAL_GET_INTERN_INCLUDE_PATH(gpip_include_path)	
	SET(${var} "${gpip_include_path}/${gpip_module_name}")
ENDMACRO(INTERNAL_GET_PUBLIC_INCLUDE_PATH)


MACRO(INTERNAL_GET_MODULE_NAME var)
	GET_FILENAME_COMPONENT(${var} ${CMAKE_CURRENT_SOURCE_DIR} NAME)
ENDMACRO(INTERNAL_GET_MODULE_NAME)


MACRO(INTERNAL_GET_INTERN_INCLUDE_PATH var)
	SET(${var} ${CMAKE_CURRENT_SOURCE_DIR}/include)
ENDMACRO(INTERNAL_GET_INTERN_INCLUDE_PATH)


MACRO(INTERNAL_GET_SOURCE_PATH var)
	SET(${var} ${CMAKE_CURRENT_SOURCE_DIR}/src)
ENDMACRO(INTERNAL_GET_SOURCE_PATH)


MACRO(INTERNAL_GET_TEST_PATH var)
	SET(${var} ${CMAKE_CURRENT_SOURCE_DIR}/test)
ENDMACRO(INTERNAL_GET_TEST_PATH)


MACRO(INTERNAL_GROUP_LINK igl_libs)
	IF(${CMAKE_COMPILER_IS_GNUCXX} OR ${CMAKE_COMPILER_IS_GNUCC})
		SET(${igl_libs} "-Wl,--start-group" ${ARGN} "-Wl,--end-group")
	ENDIF()
ENDMACRO(INTERNAL_GROUP_LINK)

MACRO(INTERNAL_LIST_REMOVE_DUPLICATES _list)
	LIST(LENGTH "${_list}" ListLength)
	IF(NOT  ${ListLength} EQUAL 0)
		LIST(REMOVE_DUPLICATES "${_list}")
	ENDIF()
ENDMACRO(INTERNAL_LIST_REMOVE_DUPLICATES)

MACRO(INTERNAL_LIST_REMOVE_ITEM _list)
	LIST(LENGTH _list ListLength)
	IF(NOT  ${ListLength} EQUAL 0)
		INTERNAL_LIST_REMOVE_ITEM(_list ${ARGN})
	ENDIF()
ENDMACRO(INTERNAL_LIST_REMOVE_ITEM)



