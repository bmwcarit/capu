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

SET(CONFIG_CREATE_FORMAT_SOURCE_CODE   0   CACHE BOOL     "add FormatSource target with astyle")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

PRINT_DETAILS(STATUS "...requires external tool Astyle")
IF(CONFIG_CREATE_FORMAT_SOURCE_CODE)
	IF(NOT ASTYLE_FOUND) 
		INCLUDE(${ACME_PATH}/tools/FindAstyle.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

IF(CONFIG_CREATE_FORMAT_SOURCE_CODE)
    IF(ASTYLE_FOUND)
        SET(DESIRED_TYPES ${GLOBAL_HEADER_FILE_EXTENSIONS} ${GLOBAL_SOURCE_FILE_EXTENSIONS})

        SET(FILE_TYPES "")

        FOREACH(DESIRED_TYPE ${DESIRED_TYPES})
            FILE(GLOB_RECURSE TMP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.${DESIRED_TYPE}")
            if(TMP_FILES)
                LIST(APPEND FILE_TYPES "*.${DESIRED_TYPE}")
            endif()
        ENDFOREACH()
		
        get_filename_component(STYLE_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
        add_custom_target(FormatSource COMMENT "Formatting the project source code" )
        add_custom_command(TARGET FormatSource COMMAND astyle --options=${STYLE_PATH}/astyle.options --recursive ${FILE_TYPES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})    
    ENDIF()
ENDIF()
