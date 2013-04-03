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

SET(CONFIG_CHECK_FOR_UNUSED_FILES   0  CACHE BOOL     "Checks for and warns about unused files in include and src directories")

#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_CHECK_FOR_UNUSED_FILES)
	ADD_PLUGIN_AFTER_DOIT_HOOK(INTERNAL_CHECK_FOR_UNUSED_FILES)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

MACRO(INTERNAL_CHECK_FOR_UNUSED_FILES)
    IF(CONFIG_CHECK_FOR_UNUSED_FILES)
        SET (path "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}")
        SET (modulename "${CURRENT_MODULE_NAME}")
		STRING(TOUPPER ${modulename} CURRENT_UPPER_MODULE_NAME)
		SET(AddedFiles ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES})
		FILE(GLOB_RECURSE publicHeaders "${path}/include/${modulename}/*.*")
		FILE(GLOB_RECURSE privateHeaders "${path}/include/*.*")
		FILE(GLOB_RECURSE sourceFiles "${path}/src/*.*")
		SET(AllFiles ${privateHeaders} ${publicHeaders} ${sourceFiles})
		FOREACH(file ${AllFiles})
			list(FIND AddedFiles ${file} normalFileFound)
			IF(${normalFileFound} EQUAL -1)
				MESSAGE(WARNING ${file} "(0): warning: File not added to acme") #  shows line in visual studio (error console)
			ENDIF()
		ENDFOREACH()
    ENDIF()
ENDMACRO(INTERNAL_CHECK_FOR_UNUSED_FILES)
