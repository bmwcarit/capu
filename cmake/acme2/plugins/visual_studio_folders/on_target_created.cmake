############################################################################
#
# Copyright 2014 BMW Car IT GmbH
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

# configure project groups in Visual Studio

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# extract folder name from path
STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" relative_path "${CMAKE_CURRENT_SOURCE_DIR}")
STRING(REGEX REPLACE "/[^/]*$" "" folder_path "${relative_path}")

IF (NOT "${relative_path}" STREQUAL "")
    IF(TARGET ${ACME_NAME})
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY FOLDER "${folder_path}")
    ENDIF()
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_SOURCE)
#==============================================================================================
    SOURCE_GROUP(Source FILES ${ACME_FILES_SOURCE})
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_GENERATED)
#==============================================================================================
    SOURCE_GROUP(Generated FILES ${ACME_FILES_GENERATED})
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_PUBLIC_HEADER)
#==============================================================================================
    SOURCE_GROUP(Public_Header FILES ${ACME_FILES_PUBLIC_HEADER})
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_PRIVATE_HEADER)
#==============================================================================================
    SOURCE_GROUP(Private_Header FILES ${ACME_FILES_PRIVATE_HEADER})
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_RESOURCE)
#==============================================================================================
    SOURCE_GROUP(Resources FILES ${ACME_FILES_RESOURCE})
ENDIF()
