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

SET(PACKAGE_NAME Socket)

IF("${TARGET_OS}" STREQUAL "Windows")
    
    SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
    SET(${PACKAGE_NAME}_LIBRARIES  "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")
            
ELSEIF("${TARGET_OS}" STREQUAL "Linux")

    SET(${PACKAGE_NAME}_INCLUDE_DIRS "" )
    SET(${PACKAGE_NAME}_LIBRARIES "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")

ELSEIF("${TARGET_OS}" STREQUAL "MacOSX")

    SET(${PACKAGE_NAME}_INCLUDE_DIRS "" )
    SET(${PACKAGE_NAME}_LIBRARIES "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")

ELSEIF("${TARGET_OS}" STREQUAL "iPhoneOS")

    SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
    SET(${PACKAGE_NAME}_LIBRARIES  "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")
    
ELSEIF("${TARGET_OS}" STREQUAL "iPhoneSimulatorOS")

    SET(${PACKAGE_NAME}_INCLUDE_DIRS "")
    SET(${PACKAGE_NAME}_LIBRARIES  "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")

ELSEIF("${TARGET_OS}" STREQUAL "Android")

    SET(${PACKAGE_NAME}_INCLUDE_DIRS "" )
    SET(${PACKAGE_NAME}_LIBRARIES "" )
    SET(${PACKAGE_NAME}_FOUND "TRUE")

ELSEIF("${TARGET_OS}" STREQUAL "Integrity")    

    FIND_PATH(${PACKAGE_NAME}_INCLUDE_DIRS /sys/socket.h /INTEGRITY-include )
    FIND_LIBRARY(${PACKAGE_NAME}_LIBRARIES NAMES socket PATHS /intarmv7a_vfp_common )
    SET(${PACKAGE_NAME}_FOUND ${${PACKAGE_NAME}_LIBRARIES}_FOUND)
    
ELSEIF("${TARGET_OS}" STREQUAL "QNX")    

    FIND_PATH(${PACKAGE_NAME}_INCLUDE_DIRS /sys/socket.h /include )
    FIND_LIBRARY(${PACKAGE_NAME}_LIBRARIES NAMES socket PATHS /x86/lib )
    SET(${PACKAGE_NAME}_FOUND ${${PACKAGE_NAME}_LIBRARIES}_FOUND)
    
ENDIF()

