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

# print module configuration during cmake run
# name is already printed by default
# we add some module information for the user

#==============================================================================================
# print build type
#==============================================================================================

STRING(TOLOWER "${ACME_TYPE}" tmp)
STRING(REGEX REPLACE "_" " " build_type_lower "${tmp}")

ACME_INFO("        type:  ${build_type_lower}")


#==============================================================================================
# print file content of module
#==============================================================================================
SET(ACME_FILES_PRIVATE_HEADER_COUNT 0)
SET(ACME_FILES_RESOURCE_COUNT       0)
SET(ACME_FILES_PUBLIC_HEADER_COUNT  0)
SET(ACME_FILES_GENERATED_COUNT      0)
SET(ACME_FILES_SOURCE_COUNT         0)

LIST(LENGTH ACME_FILES_SOURCE         ACME_FILES_SOURCE_COUNT)
LIST(LENGTH ACME_FILES_GENERATED      ACME_FILES_GENERATED_COUNT)
LIST(LENGTH ACME_FILES_PUBLIC_HEADER  ACME_FILES_PUBLIC_HEADER_COUNT)
LIST(LENGTH ACME_FILES_PRIVATE_HEADER ACME_FILES_PRIVATE_HEADER_COUNT)
LIST(LENGTH ACME_FILES_RESOURCE       ACME_FILES_RESOURCE_COUNT)

ACME_INFO("        files: ${ACME_FILES_SOURCE_COUNT} src, ${ACME_FILES_GENERATED_COUNT} gen, ${ACME_FILES_PUBLIC_HEADER_COUNT} h_pub, ${ACME_FILES_PRIVATE_HEADER_COUNT} h_priv, ${ACME_FILES_RESOURCE_COUNT} res")


#==============================================================================================
# print depedencies of module
#==============================================================================================
IF(DEFINED ACME_DEPENDENCIES AND TARGET ${ACME_NAME})
    ACME_INFO("        deps:  ${ACME_DEPENDENCIES}")
ENDIF()

IF(DEFINED ACME_DEPENDENCIES_SYSTEM AND TARGET ${ACME_NAME})
    ACME_INFO(" system deps:  ${ACME_DEPENDENCIES_SYSTEM}")
ENDIF()

ACME_INFO("")
