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

# Adds an ExportTar target to the project
# The generated Tar File in deliverable contains everything included through new Commands
# ACME_EXPORT_DIRECORY dir (target)
# ACME_EXPORT_TARGET targetname (without args uses current target)
# ACME_EXPORT_FILE filename (target)
# Frist parameters are relativ to cmake file containing command, optional targetParameter specifies path within Tar


#--------------------------------------------------------------------------
# Plugin API
#--------------------------------------------------------------------------

FUNCTION(ACME_EXPORT_DIRECTORY sourceDir)
	INTERNAL_EXPORT_DIRECTORY( ${sourceDir} "${ARGV1}")
ENDFUNCTION(ACME_EXPORT_DIRECTORY)

FUNCTION(ACME_EXPORT_TARGET)
	INTERNAL_EXPORT_TARGET(${ARGV0})
ENDFUNCTION(ACME_EXPORT_TARGET)

FUNCTION(ACME_EXPORT_FILE filename)
	INTERNAL_ACME_EXPORT_FILE(${filename} ${ARGV1})
ENDFUNCTION(ACME_EXPORT_FILE)


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

ADD_PLUGIN_FINALIZE_HOOK(INTERNAL_BUILD_RELEASE_TAR)


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

SET(TARGETS_TO_COPY "" CACHE INTERNAL "")
SET(FILES_TO_COPY "" CACHE INTERNAL "")
SET(DIRECTORIES_TO_COPY "" CACHE INTERNAL "")

FUNCTION(INTERNAL_EXPORT_TARGET)
	IF(NOT ${ARGV0} STREQUAL "")
		SET(targetDirectoryPath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}/${ARGV0}")
	ELSE()
		SET(targetDirectoryPath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}")
	ENDIF()
	SET(${CURRENT_MODULE_NAME}_TARGETDIRS ${${CURRENT_MODULE_NAME}_TARGETDIRS} ${targetDirectoryPath} CACHE INTERNAL "")
	SET(TARGETS_TO_COPY ${TARGETS_TO_COPY} ${CURRENT_MODULE_NAME} CACHE INTERNAL "")
ENDFUNCTION(INTERNAL_EXPORT_TARGET)


FUNCTION(INTERNAL_EXPORT_DIRECTORY sourceDir) # optional parameter targetdir
	IF(NOT ${ARGV1} STREQUAL "")
		SET(targetDirectoryPath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}/${ARGV1}/")
	ELSE()
		SET(targetDirectoryPath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}/${sourceDir}/")
	ENDIF()
	SET(sourceDirectoryPath ${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir})
	SET(${sourceDirectoryPath}_TARGETDIRS ${${sourceDirectoryPath}_TARGETDIRS} ${targetDirectoryPath} CACHE INTERNAL "")
	SET(DIRECTORIES_TO_COPY ${DIRECTORIES_TO_COPY} ${sourceDirectoryPath} CACHE INTERNAL "")
ENDFUNCTION(INTERNAL_EXPORT_DIRECTORY)


FUNCTION(INTERNAL_ACME_EXPORT_FILE sourceFilename)
	IF(NOT ${ARGV1} STREQUAL "")
		SET(targetFilePath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}/${ARGV1}")
	ELSE()
		SET(targetFilePath "${GLOBAL_TOP_LEVEL_BINARY_DIR}/export/${PROJECT_NAME}/${sourceFilename}")
	ENDIF()
	SET(sourceFilePath ${CMAKE_CURRENT_SOURCE_DIR}/${sourceFilename})

	SET(${sourceFilePath}_TARGETPATHS ${${sourceFilePath}_TARGETPATHS} ${targetFilePath} CACHE INTERNAL "")
	SET(FILES_TO_COPY ${FILES_TO_COPY} ${sourceFilePath} CACHE INTERNAL "")
ENDFUNCTION(INTERNAL_ACME_EXPORT_FILE)


# collect everything for tar building
FUNCTION(INTERNAL_BUILD_RELEASE_TAR)
	MESSAGE(STATUS "Adding Tarfile export target")
	# the top level target for building the tar
	add_custom_target(ExportTar COMMENT "Exporting marked Files into a tar" )

	# make files to copy out of targets to copy
	FOREACH(TARGET_TO_COPY ${TARGETS_TO_COPY})
		get_target_property(fullFilePath ${TARGET_TO_COPY} LOCATION)
		get_filename_component(FILENAME "${fullFilePath}" NAME)
		FOREACH(TARGET_DIR ${${TARGET_TO_COPY}_TARGETDIRS})
			add_custom_command(TARGET ExportTar COMMAND ${CMAKE_COMMAND} -E copy_if_different "${fullFilePath}" "${TARGET_DIR}/${FILENAME}")
		ENDFOREACH()
	ENDFOREACH()

	# go through all directories to be copied
	FOREACH(DIRECTORY_TO_COPY ${DIRECTORIES_TO_COPY})
		FOREACH(TARGET_DIR ${${DIRECTORY_TO_COPY}_TARGETDIRS})
			add_custom_command(TARGET ExportTar COMMAND ${CMAKE_COMMAND} -E copy_directory "${DIRECTORY_TO_COPY}" "${TARGET_DIR}")
		ENDFOREACH()
	ENDFOREACH()

	# go through all files to include in tar
	FOREACH(FILE_TO_COPY ${FILES_TO_COPY})
		FOREACH(TARGET_PATH ${${FILE_TO_COPY}_TARGETPATHS})
			add_custom_command(TARGET ExportTar COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILE_TO_COPY}" "${TARGET_PATH}")
		ENDFOREACH()
	ENDFOREACH()

	# tar everything beneath build/export dir
	add_custom_command(TARGET ExportTar COMMAND ${CMAKE_COMMAND} -E tar cvf ${PROJECT_NAME}.tar "${PROJECT_NAME}" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/export/)

	# copy tar to deliverable folder
	add_custom_command(TARGET ExportTar COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/export/${PROJECT_NAME}.tar" "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.tar")
ENDFUNCTION(INTERNAL_BUILD_RELEASE_TAR)
