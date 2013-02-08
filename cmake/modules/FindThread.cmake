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

# To specify your package use the following variables
# ${PACKAGE_NAME}_INCLUDE_DIRS
# ${PACKAGE_NAME}_LIBRARY_DIRS
# ${PACKAGE_NAME}_LIBNAMES
# ${PACKAGE_NAME}_DEPENDENT_DEFINITIONS
# ${PACKAGE_NAME}_DEPENDENT_DEBUG_DEFINITIONS
# ${PACKAGE_NAME}_DEPENDENT_RELEASE_DEFINITIONS 

SET(PACKAGE_NAME Thread)

IF("${TARGET_OS}" STREQUAL "Windows")
    
	SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
	SET(${PACKAGE_NAME}_LIBRARIES  "" )
            
ELSEIF("${TARGET_OS}" STREQUAL "Linux")

	find_package (Threads)
	IF("${Threads_FOUND}" STREQUAL "TRUE")
		SET(${PACKAGE_NAME}_FOUND ${Threads_FOUND})
		SET(${PACKAGE_NAME}_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
		SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
	ENDIF()

ELSEIF("${TARGET_OS}" STREQUAL "MacOSX")

	SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
	SET(${PACKAGE_NAME}_LIBRARIES  "" )

ELSEIF("${TARGET_OS}" STREQUAL "Integrity")	

	FIND_PATH(${PACKAGE_NAME}_INCLUDE_DIRS pthread.h /INTEGRITY-include )
	FIND_LIBRARY(${PACKAGE_NAME}_LIBRARIES NAMES posix PATHS /intarmv7a_vfp_common )
	
ELSEIF("${TARGET_OS}" STREQUAL "QNX")	

	FIND_PATH(${PACKAGE_NAME}_INCLUDE_DIRS pthread.h /include )
	FIND_LIBRARY(${PACKAGE_NAME}_LIBRARIES NAMES c PATHS /x86/lib )
	
ENDIF()

