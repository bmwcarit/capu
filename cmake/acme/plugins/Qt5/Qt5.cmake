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

SET(CONFIG_USE_QT5	  0	 CACHE BOOL	   "enable the support of QT5")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

MESSAGE(VERBOSE "...requires installation of QT5")
IF(CONFIG_USE_QT5)
		
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	SET(CMAKE_AUTOMOC ON)
	
	IF(NOT QT5_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindQT5.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_USE_QT5)
	ADD_PLUGIN_AFTER_DOIT_HOOK(INTERNAL_CREATE_QT5_TARGET)
        ADD_PLUGIN_BEFORE_DOIT_HOOK(INTERNAL_QT5_RESOURCES)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------


FUNCTION(ACME_QT5_USE_MODULES module)
	SET(GLOBAL_ACME_QT5_MODULES ${module} ${ARGN} CACHE  INTERNAL "QT5 modules of current module")
ENDFUNCTION()

FUNCTION(ACME_QT5_WRAP_UI uifile)
	SET(GLOBAL_ACME_QT5_UIFILES ${uifile} ${ARGN} CACHE  INTERNAL "QT5 uifiles of current module")	
ENDFUNCTION()


FUNCTION(ACME_QT5_ADD_RESOURCE_FILE resfile)
        SET(GLOBAL_ACME_QT5_RESFILES ${resfile} ${ARGN} CACHE  INTERNAL "QT5 resfiles of current module")
ENDFUNCTION()

MACRO(INTERNAL_QT5_RESOURCES)
        STRING(TOUPPER ${CURRENT_MODULE_NAME} CURRENT_UPPER_MODULE_NAME)
        FOREACH(ACME_QT_RESFILE ${GLOBAL_ACME_QT5_RESFILES})
                SET(QT5_RESFILE "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/${ACME_QT_RESFILE}.qrc")
                IF(EXISTS ${QT5_RESFILE})
                        qt5_add_resources(ACME_QT_RESFILE_TO_LINK ${QT5_RESFILE})
                        SET(${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES ${${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES} ${QT5_RESFILE} ${ACME_QT_RESFILE_TO_LINK} CACHE INTERNAL "")
                ENDIF()
        ENDFOREACH()
        SET(GLOBAL_ACME_QT5_RESFILES "" CACHE  INTERNAL "QT5 resfiles of current module")
ENDMACRO()

# Must be a macro
MACRO(INTERNAL_CREATE_QT5_TARGET)
	IF(NOT "${GLOBAL_ACME_QT5_MODULES}" STREQUAL "")
	
		SET(CMAKE_MODULE_PATH_BACKUP "${CMAKE_MODULE_PATH}")
		SET(CMAKE_MODULE_PATH "")
		
		FOREACH(ACME_QT_MODULE ${GLOBAL_ACME_QT5_MODULES})
			find_package(Qt5${ACME_QT_MODULE} REQUIRED)	
		ENDFOREACH()
    
		qt5_use_modules(${CURRENT_MODULE_NAME} ${GLOBAL_ACME_QT5_MODULES})
		
		SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH_BACKUP}")
		
		FOREACH(ACME_QT_UIFILE ${GLOBAL_ACME_QT5_UIFILES})	
			SET(QT5_UIFILE "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_MODULE_NAME}/${ACME_QT_UIFILE}.ui")
			IF(EXISTS ${QT5_UIFILE})
				qt5_wrap_ui(ui_${ACME_QT_UIFILE}.h "${QT5_UIFILE}")
			ENDIF()
		ENDFOREACH()
    
	ENDIF()
	SET(GLOBAL_ACME_QT5_MODULES "" CACHE  INTERNAL "QT5 modules of current module")
	SET(GLOBAL_ACME_QT5_UIFILES "" CACHE  INTERNAL "QT5 uifiles of current module")

ENDMACRO()






