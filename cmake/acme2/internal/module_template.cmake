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

IF(DEFINED ACME_FILES_UNIT_TEST)
    ACME_WARNING("${ACME_NAME} defines ACME_FILES_UNIT_TEST! This feature was removed. Create separate ACME_MODULE with type TEST instead.")
ENDIF()
IF(DEFINED DEPENDENCIES_UNIT_TEST)
    ACME_WARNING("${ACME_NAME} defines DEPENDENCIES_UNIT_TEST! This feature was removed. Create separate ACME_MODULE with type TEST instead.")
ENDIF()
IF(DEFINED CONTENT_TEST)
    ACME_WARNING("${ACME_NAME} defines CONTENT_TEST! This feature was removed. Create separate ACME_MODULE with type TEST instead.")
ENDIF()


SET(TARGET_CONTENT_HEADER
    ${ACME_FILES_PUBLIC_HEADER}
    ${ACME_FILES_PRIVATE_HEADER}
)

SET(TARGET_CONTENT_SOURCE
    ${ACME_FILES_SOURCE}
    ${ACME_FILES_GENERATED}
)

SET(TARGET_CONTENT
    ${TARGET_CONTENT_HEADER}
    ${TARGET_CONTENT_SOURCE}
    ${ACME_FILES_RESOURCE}
)

IF (TARGET ${ACME_NAME})
    ACME_WARNING("Skipping target ${ACME_NAME}, already present in build env.")
    RETURN()
ENDIF()

IF(NOT "${TARGET_CONTENT}" STREQUAL "")

    #==============================================================================================
    IF("${ACME_TYPE}" STREQUAL "TEST")
    #==============================================================================================

        ADD_TEST(
            NAME ${ACME_NAME}_UNITTESTS
            COMMAND ${CMAKE_COMMAND} -E chdir "$<TARGET_FILE_DIR:${ACME_NAME}>"
                    ./${ACME_NAME} --gtest_output=xml:TestResult_${ACME_NAME}.xml
		)

    ENDIF()

    #==============================================================================================
    IF("${ACME_TYPE}" STREQUAL "STATIC_LIBRARY")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} STATIC ${TARGET_CONTENT})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND "${ACME_ENABLE_INSTALL}")
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_STATIC_LIB} COMPONENT "${ACME_PACKAGE_NAME}-devel")
        ENDIF()

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "BINARY")
    #==============================================================================================
        ADD_EXECUTABLE(${ACME_NAME} ${TARGET_CONTENT})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_BINARY} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "TEST")
    #==============================================================================================
        ADD_EXECUTABLE(${ACME_NAME} ${TARGET_CONTENT})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_BINARY} COMPONENT "${ACME_PACKAGE_NAME}-test")
        ENDIF()

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "SHARED_LIBRARY")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} SHARED ${TARGET_CONTENT})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_SHARED_LIB} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "PLUGIN")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} MODULE ${TARGET_CONTENT})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_PLUGIN} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "OBJECT_FILES")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} OBJECT ${TARGET_CONTENT})
        # object file bundles are internal and should not be installed

    #==============================================================================================
    ELSEIF()
    #==============================================================================================
        ACME_ERROR("Your module has invalid type '${ACME_TYPE}'")
    ENDIF()

    #==============================================================================================
    # set common ACME2 project properties
    #==============================================================================================
    IF(TARGET ${ACME_NAME})
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY LINKER_LANGUAGE     CXX)
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY ACME_TYPE           ${ACME_TYPE})
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY VERSION             ${ACME_VERSION})
    ENDIF()
    
    #==============================================================================================
    # internal lists
    #==============================================================================================
    SET(INCLUDE_PATH_LIST "${ACME_INCLUDE_BASE}")

ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_PUBLIC_HEADER)
#==============================================================================================
    FOREACH(INCLUDE_BASE ${ACME_INCLUDE_BASE})
        FILE(GLOB INCLUDE_BASE_FULL_PATH "${INCLUDE_BASE}")
        FOREACH(PUBLIC_HEADER ${ACME_FILES_PUBLIC_HEADER})
            STRING(REGEX REPLACE "${INCLUDE_BASE_FULL_PATH}/(.*)/[^/]*$" "include/\\1" PUBLIC_HEADER_INSTALL_PATH ${PUBLIC_HEADER})
            IF (${ACME_ENABLE_INSTALL})
                IF ("${PUBLIC_HEADER_INSTALL_PATH}" STREQUAL "${PUBLIC_HEADER}")
                    SET(PUBLIC_HEADER_INSTALL_PATH "include") #TODO: remove this hard coded "include"
                ENDIF()
                INSTALL(FILES ${PUBLIC_HEADER} DESTINATION ${PUBLIC_HEADER_INSTALL_PATH} COMPONENT "${ACME_PACKAGE_NAME}-devel")
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_RESOURCE)
#==============================================================================================
    IF(${ACME_ENABLE_INSTALL})
        INSTALL(FILES ${ACME_FILES_RESOURCE} DESTINATION ${ACME_INSTALL_RESOURCE} COMPONENT "${ACME_PACKAGE_NAME}")
    ENDIF()

    # make sure res folder exists, neccessary evil
    ADD_CUSTOM_COMMAND(
        TARGET ${ACME_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${ACME_NAME}>/res"
    )

    # add copy if different target to each file
    FOREACH(file ${ACME_FILES_RESOURCE})
        ADD_CUSTOM_COMMAND(
            TARGET ${ACME_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} "$<TARGET_FILE_DIR:${ACME_NAME}>/res/"
        )
    ENDFOREACH()

ENDIF()

#==============================================================================================
IF(DEFINED ACME_DEPENDENCIES
   AND TARGET ${ACME_NAME})
#==============================================================================================
    FOREACH(DEPENDENCY ${ACME_DEPENDENCIES})

        IF (NOT "${${DEPENDENCY}_INCLUDE_DIRS}" STREQUAL "")
            LIST(APPEND INCLUDE_PATH_LIST "${${DEPENDENCY}_INCLUDE_DIRS}")
        ENDIF()

        IF(TARGET ${DEPENDENCY})

            ADD_DEPENDENCIES(${ACME_NAME} ${DEPENDENCY})

            # dependency is part of this project, query dependency properties

            GET_TARGET_PROPERTY(acme_type      ${DEPENDENCY} ACME_TYPE)
            GET_TARGET_PROPERTY(cmake_type     ${DEPENDENCY} TYPE)
            GET_TARGET_PROPERTY(contains_files ${DEPENDENCY} SOURCES)

            IF("${acme_type}" STREQUAL "BINARY")
                TARGET_LINK_LIBRARIES(${ACME_NAME} $<TARGET_OBJECTS:${DEPENDENCY}>)

            ELSEIF("${acme_type}" STREQUAL "TEST")
                TARGET_LINK_LIBRARIES(${ACME_NAME} $<TARGET_OBJECTS:${DEPENDENCY}>)

            ELSEIF("${acme_type}" STREQUAL "OBJECT_FILES")
                TARGET_LINK_LIBRARIES(${ACME_NAME} $<TARGET_OBJECTS:${DEPENDENCY}>)

            ELSEIF("${cmake_type}" STREQUAL "UTILITY")
                IF (NOT "${${DEPENDENCY}_LIBRARIES}" STREQUAL "")
                    TARGET_LINK_LIBRARIES(${ACME_NAME} ${${DEPENDENCY}_LIBRARIES})
                ENDIF()

            ELSEIF(NOT "${contains_files}" STREQUAL "")
                TARGET_LINK_LIBRARIES(${ACME_NAME} ${DEPENDENCY})
            ENDIF()

        ELSE()

            # dependency is not part of this project, use CMAKE to resolve dependencies

            IF(ACME_ENABLE_DEPENDENCY_CHECK)
                IF(NOT ${DEPENDENCY}_FOUND)
                    ACME_ERROR("required dependency '${DEPENDENCY}' not found!")
                ENDIF()
            ENDIF()

            IF(NOT "${${DEPENDENCY}_LIBRARIES}" STREQUAL "")
                TARGET_LINK_LIBRARIES(${ACME_NAME} ${${DEPENDENCY}_LIBRARIES})
            ENDIF()

        ENDIF()

    ENDFOREACH()

ENDIF()

#==============================================================================================
IF(DEFINED ACME_DEPENDENCIES_HEADER
   AND TARGET ${ACME_NAME})
#==============================================================================================
    FOREACH(HEADER_DEPENDENCY ${ACME_DEPENDENCIES_HEADER})
        ADD_DEPENDENCIES(${ACME_NAME} ${HEADER_DEPENDENCY})
        IF(NOT "${${HEADER_DEPENDENCY}_INCLUDE_DIRS}" STREQUAL "")
            LIST(APPEND INCLUDE_PATH_LIST "${${HEADER_DEPENDENCY}_INCLUDE_DIRS}")
        ENDIF()
    ENDFOREACH()
ENDIF()


#==============================================================================================
# set all required include paths of target
#==============================================================================================
IF(TARGET ${ACME_NAME} AND NOT "${INCLUDE_PATH_LIST}" STREQUAL "")
    LIST(REMOVE_DUPLICATES INCLUDE_PATH_LIST)
    SET_TARGET_PROPERTIES(${ACME_NAME} PROPERTIES INCLUDE_DIRECTORIES "${INCLUDE_PATH_LIST}")
ENDIF()

