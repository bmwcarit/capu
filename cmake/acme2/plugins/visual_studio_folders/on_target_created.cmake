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


#==========================================================================
# configure project groups in Visual Studio
#==========================================================================

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# extract folder name from path
STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" relative_path "${CMAKE_CURRENT_SOURCE_DIR}")
STRING(REGEX REPLACE "/[^/]*$" "" folder_path "${relative_path}")

IF (NOT "${relative_path}" STREQUAL "")
    IF(TARGET ${ACME_NAME})
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY FOLDER "${folder_path}")
    ENDIF()
ENDIF()

IF(TARGET ACME2_COPY_RESOURCES_${ACME_NAME})
    SET_PROPERTY(TARGET ACME2_COPY_RESOURCES_${ACME_NAME} PROPERTY FOLDER "CMakePredefinedTargets/ACME2_COPY_RESOURCES")
ENDIF()

IF(TARGET ACME2_CPDLL_${ACME_NAME})
    SET_PROPERTY(TARGET ACME2_CPDLL_${ACME_NAME} PROPERTY FOLDER "CMakePredefinedTargets/ACME2_COPY_SHARED_LIBS")
ENDIF()

#==========================================================================
# configure project internal structure
#==========================================================================

SET(all_files
    ${ACME_FILES_SOURCE}
    ${ACME_FILES_GENERATED}
    ${ACME_FILES_PUBLIC_HEADER}
    ${ACME_FILES_PRIVATE_HEADER}
    ${ACME_FILES_RESOURCE}
)

FOREACH(file_iter ${all_files})
    STRING(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" tmp1 "${file_iter}")
    STRING(REGEX REPLACE "/[^/]*$" "" tmp2 "${tmp1}")
    STRING(REPLACE "/" "\\" module_internal_path "${tmp2}")
    SOURCE_GROUP(${module_internal_path} FILES ${file_iter})
ENDFOREACH()
