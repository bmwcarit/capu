############################################################################
#
# Copyright (C) 2014 BMW Car IT GmbH
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

#==============================================================================
# logging
#==============================================================================

MACRO(ACME_INFO)
    MESSAGE(STATUS "${ARGV}")
ENDMACRO()

MACRO(ACME_DEBUG)
    IF(${ACME_DEBUG_ENABLED})
        MESSAGE(STATUS "${ARGV}")
    ENDIF()
ENDMACRO()

MACRO(ACME_WARNING)
    IF(${ACME_WARNING_AS_ERROR})
        ACME_ERROR("${ARGV}")
    ELSE()
        MESSAGE("        ${ARGV}")
    ENDIF()
ENDMACRO()

MACRO(ACME_ERROR)
    ACME_INFO("ERROR: ${ARGV}")
    MESSAGE(FATAL_ERROR "Cancel Build.")
ENDMACRO()


#==============================================================================
# argument splitter
#==============================================================================

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

MACRO(ARGUMENT_SPLITTER argInput argKeywordList argPrefix)

    SET(inputTokenList "${argInput}")
    SET(keywordList ${argKeywordList})
    SET(splitter_prefix ${argPrefix})

    # initialize all out variables to default values
    FOREACH(keyword ${keywordList})
        SET(${splitter_prefix}${keyword}_FOUND 0)
        SET(${splitter_prefix}${keyword} "")
    ENDFOREACH()

    # iterate all tokens of provided input
    FOREACH(token ${inputTokenList})
        # check if current token is a keyword
        LIST(FIND keywordList ${token} tokenIsKeyword)

        IF (NOT ${tokenIsKeyword} EQUAL -1)
            # if current token is keyword, set found variable to true
            SET(lastKeyword ${token})
            SET(${splitter_prefix}${token}_FOUND 1)
        ELSE ()
            # if current token is not keyword, append token to variable of last found keyword
            LIST(APPEND ${splitter_prefix}${lastKeyword} ${token})
        ENDIF ()
    ENDFOREACH()
    
ENDMACRO(ARGUMENT_SPLITTER)


#==============================================================================
# file list resolver
#==============================================================================
MACRO(GET_ALL_FILES var_name directory_list)
    SET(file_list "")
    FOREACH(directory ${directory_list})
        FILE(GLOB directory_content "${directory}")
        LIST(APPEND file_list ${directory_content})
    ENDFOREACH()
    SET(${var_name} ${file_list})
ENDMACRO(GET_ALL_FILES)


#==============================================================================
# set default values, if no value is set
#==============================================================================
MACRO(DEFAULT_VALUE var_name value)
    IF("${${var_name}}" STREQUAL "")
        SET(${var_name} ${value})
    ENDIF()
    IF(NOT ${var_name})
        SET(${var_name} ${value})
    ENDIF()
ENDMACRO()
