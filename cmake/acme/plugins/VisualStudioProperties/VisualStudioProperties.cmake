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

SET(CONFIG_VISUAL_STUDIO_PROPERTIES   0  CACHE BOOL     "visual studio properties")

IF (MSVC)
	#Plugin only relevant for Visual Studio
	SET(CONFIG_VISUAL_STUDIO_PROPERTIES   1  CACHE BOOL     "visual studio properties")
ENDIF()
SET(CONFIG_VISUAL_STUDIO_DEBUGGER_DIRECTORY   $(TargetDir)  CACHE STRING     "visual studio debugger properties")

#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_VISUAL_STUDIO_PROPERTIES)
	ADD_PLUGIN_AFTER_DOIT_HOOK(INTERNAL_COPY_PROPERTY_FILE)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_COPY_PROPERTY_FILE)
	MESSAGE(VERBOSE VISUAL_STUDIO_PROPERTIES_PLUGIN "copy from ${ACME_PATH}/plugins/VisualStudioProperties/Target.vcxproj.user to ${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_MODULE_NAME}")
	CONFIGURE_FILE(${ACME_PATH}/plugins/VisualStudioProperties/Target.vcxproj.user ${CMAKE_CURRENT_BINARY_DIR})
	FILE(RENAME ${CMAKE_CURRENT_BINARY_DIR}/Target.vcxproj.user ${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_MODULE_NAME}.vcxproj.user)
ENDFUNCTION(INTERNAL_COPY_PROPERTY_FILE)

