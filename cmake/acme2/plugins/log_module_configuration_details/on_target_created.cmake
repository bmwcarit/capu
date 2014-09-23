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

# print module configuration during cmake run
# name is already printed by default
# we add some module information for the user

#==============================================================================================
# print dependencies
#==============================================================================================
IF(DEFINED ACME_DEPENDENCIES AND TARGET ${ACME_NAME})
    FOREACH(ACME_DEPENDENCY ${ACME_DEPENDENCIES})

        IF(TARGET ${ACME_DEPENDENCY})

            GET_TARGET_PROPERTY(acme_type           ${ACME_DEPENDENCY} ACME_TYPE)
            ACME_INFO("        depends on ${ACME_DEPENDENCY} (CMAKE internal)")
            ACME_INFO("                INCLUDE    ${${ACME_DEPENDENCY}_INCLUDE_DIRS}")

            IF("${acme_type}" STREQUAL "BINARY")
                ACME_INFO("                OBJ        obj files from ${ACME_DEPENDENCY}")
            ELSEIF("${acme_type}" STREQUAL "OBJECT_FILES")
                ACME_INFO("                OBJ        obj files from ${ACME_DEPENDENCY}")
            ELSE()
                ACME_INFO("                LIB        ${ACME_DEPENDENCY}")
            ENDIF()
        ELSE()
            ACME_INFO("        depends on ${ACME_DEPENDENCY} (${CMAKE_MODULE_PATH}/Find${ACME_DEPENDENCY}.cmake)")
            ACME_INFO("                INCLUDE    ${${ACME_DEPENDENCY}_INCLUDE_DIRS}")
            ACME_INFO("                LIBS       ${${ACME_DEPENDENCY}_LIBRARIES}")
        ENDIF()
    ENDFOREACH()
ENDIF()


#==============================================================================================
# print header dependencies
#==============================================================================================
IF(DEFINED ACME_DEPENDENCIES_HEADER AND TARGET ${ACME_NAME})
    FOREACH(ACME_DEPENDENCY_HEADER ${ACME_DEPENDENCIES_HEADER})
        ACME_INFO ("        depends on ${ACME_DEPENDENCY_HEADER} headers (CMAKE internal)")
        ACME_DEBUG("                INCLUDE    ${${ACME_DEPENDENCY}_INCLUDE_DIRS}")
    ENDFOREACH()
ENDIF()

#==============================================================================================
# print all variables of project
#==============================================================================================
FOREACH(PROPERTY ${ACME2_API})
    ACME_INFO("        ${PROPERTY}")
    ACME_INFO("                     DEFAULT = ${DEFAULT_${PROPERTY}}")
    ACME_INFO("                     PROJECT = ${PROJECT_${PROPERTY}}")
    ACME_INFO("                     MODULE  = ${MODULE_${PROPERTY}}")
    ACME_INFO("                     ACME    = ${ACME_${PROPERTY}}")
ENDFOREACH()
