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

# List TARGET_OBJECTS in SOURCES target property.
IF (POLICY CMP0051)
    CMAKE_POLICY(SET CMP0051 OLD)
ENDIF()

MACRO(ADD_DEPENDENCIES_TO_TARGET TARGETNAME DEPENDENCIESTOADD IS_SYSTEM_DEPENDENCY)
    FOREACH(DEPENDENCY ${DEPENDENCIESTOADD})
        IF(TARGET ${DEPENDENCY})
            GET_TARGET_PROPERTY(${DEPENDENCY}_INCLUDE_DIRS ${DEPENDENCY} INCLUDE_DIRECTORIES)
        ENDIF()

        IF (NOT "${${DEPENDENCY}_INCLUDE_DIRS}" STREQUAL "")
            IF (${IS_SYSTEM_DEPENDENCY})
                LIST(APPEND SYSTEM_INCLUDE_PATH_LIST "${${DEPENDENCY}_INCLUDE_DIRS}")
            ELSE()
                LIST(APPEND INCLUDE_PATH_LIST "${${DEPENDENCY}_INCLUDE_DIRS}")
            ENDIF()
        ENDIF()

        GET_TARGET_PROPERTY(TARGET_TYPE      ${TARGETNAME} ACME_TYPE)

        IF(TARGET ${DEPENDENCY})

            ADD_DEPENDENCIES(${TARGETNAME} ${DEPENDENCY})

            # dependency is part of this project, query dependency properties

            GET_TARGET_PROPERTY(DEPENDENCY_TYPE      ${DEPENDENCY} ACME_TYPE)
            GET_TARGET_PROPERTY(cmake_type     ${DEPENDENCY} TYPE)
            GET_TARGET_PROPERTY(contains_files ${DEPENDENCY} SOURCES)

            IF (("${TARGET_TYPE}" STREQUAL "CUSTOM_TEST_TARGET") OR ("${TARGET_TYPE}" STREQUAL "CUSTOM_TARGET"))
                # cant link anything to custom targets
            ELSEIF(NOT "${contains_files}" STREQUAL "")
                IF (("${DEPENDENCY_TYPE}" STREQUAL "CUSTOM_TEST_TARGET") OR ("${DEPENDENCY_TYPE}" STREQUAL "CUSTOM_TARGET") OR ("${DEPENDENCY_TYPE}" STREQUAL "OBJECT"))
                    # jump over dependencies of this type, and link their dependencies instead
                    GET_TARGET_PROPERTY(DEPENDENCY_DEPENDENCIES      ${DEPENDENCY} ACME_DEPENDENCIES)
                    IF(DEPENDENCY_DEPENDENCIES)
                        ADD_DEPENDENCIES_TO_TARGET("${TARGETNAME}" "${DEPENDENCY_DEPENDENCIES}" ${IS_SYSTEM_DEPENDENCY})
                    ENDIF()
                ELSE()
                    IF ( ("${TARGET_TYPE}" STREQUAL "SHARED_LIBRARY") )
                        TARGET_LINK_LIBRARIES(${TARGETNAME} LINK_PRIVATE ${DEPENDENCY})
                    ELSEIF ("${TARGET_TYPE}" STREQUAL "OBJECT")
                    ELSE()
                        TARGET_LINK_LIBRARIES(${TARGETNAME} LINK_PUBLIC ${DEPENDENCY})
                    ENDIF()
                ENDIF()
            ENDIF()

        ELSE()

            # dependency is not part of this project, use CMAKE to resolve dependencies

            IF(ACME_ENABLE_DEPENDENCY_CHECK)
                IF(NOT ${DEPENDENCY}_FOUND)
                    FIND_PACKAGE(${DEPENDENCY} QUIET)
                    IF(NOT ${DEPENDENCY}_FOUND)
                        ACME_ERROR("required dependency '${DEPENDENCY}' not found!")
                    ENDIF()
                ENDIF()
            ENDIF()

            IF(NOT "${${DEPENDENCY}_LIBRARIES}" STREQUAL "")
                IF (NOT "${TARGET_TYPE}" STREQUAL "OBJECT")
                    TARGET_LINK_LIBRARIES(${TARGETNAME} LINK_PUBLIC ${${DEPENDENCY}_LIBRARIES})
                ENDIF()
            ENDIF()

        ENDIF()

    ENDFOREACH()

ENDMACRO()


FUNCTION(COLLECT_DLL_DEPENDENCIES_FOR_TARGET TARGETNAME RESULT_DLL_TARGETS)
  # only collect for executables
  GET_TARGET_PROPERTY(TARGET_TYPE ${TARGETNAME} ACME_TYPE)
  IF ("${TARGET_TYPE}" STREQUAL "BINARY" OR "${TARGET_TYPE}" STREQUAL "TEST")
    # do a breadth first search of all targets, start with TARGETNAME and detect
    # all dlls in the whole search space
    LIST(APPEND TARGETS_SCANNED ${TARGETNAME})
    LIST(APPEND TARGETS_TO_SCAN ${TARGETNAME})
    LIST(APPEND DLL_TARGETS)

    WHILE (TRUE)
      # end when all targets scanned
      LIST(LENGTH TARGETS_TO_SCAN LEN)
      IF (${LEN} EQUAL 0)
        BREAK()
      ENDIF()

      # pop last
      MATH(EXPR LAST_IDX "${LEN}-1")
      LIST(GET TARGETS_TO_SCAN ${LAST_IDX} CURRENT_TARGET)
      LIST(REMOVE_AT TARGETS_TO_SCAN ${LAST_IDX})

      # add to result if is dll
      GET_TARGET_PROPERTY(TARGET_TYPE ${CURRENT_TARGET} ACME_TYPE)
      IF ("${TARGET_TYPE}" STREQUAL "SHARED_LIBRARY")
        LIST(APPEND DLL_TARGETS ${CURRENT_TARGET})
      ENDIF()

      # iterate over dependencies
      GET_TARGET_PROPERTY(TARGET_DEPENDENCIES ${CURRENT_TARGET} ACME_DEPENDENCIES)
      FOREACH (DEP_TARGET IN LISTS TARGET_DEPENDENCIES)
        IF (TARGET ${DEP_TARGET})
          # if not yet visited add to scan list
          LIST(FIND TARGETS_SCANNED ${DEP_TARGET} DEP_TARGET_KNOWN)
          IF (${DEP_TARGET_KNOWN} LESS 0)
            LIST(APPEND TARGETS_TO_SCAN ${DEP_TARGET})
          ENDIF()

          # remember as already visited
          LIST(APPEND TARGETS_SCANNED ${DEP_TARGET})
        ENDIF()
      ENDFOREACH()

    ENDWHILE()
  ENDIF()

  # store result list, may be empty
  SET(${RESULT_DLL_TARGETS} ${DLL_TARGETS} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(ADD_DLL_COPY_TARGET_TO_TARGET TARGETNAME DLL_TARGETS)
  IF (DLL_TARGETS)
    # construct multiple copy commands to paste into one custom command (incl comment)
    SET(MULTIPLE_COPY_COMMANDS)
    SET(MULTIPLE_COPY_COMMENT_TARGETS "")
    FOREACH (DLL_TARGET_NAME IN LISTS DLL_TARGETS)
      LIST(APPEND MULTIPLE_COPY_COMMANDS COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${DLL_TARGET_NAME}>" ${CMAKE_CURRENT_BINARY_DIR})
      SET(MULTIPLE_COPY_COMMENT_TARGETS "${MULTIPLE_COPY_COMMENT_TARGETS}${DLL_TARGET_NAME} ")
    ENDFOREACH()

    # custom copy command
    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}-CopiedSharedLibraries
      ${MULTIPLE_COPY_COMMANDS}
      COMMENT "Copying ${MULTIPLE_COPY_COMMENT_TARGETS}-> ${CMAKE_CURRENT_BINARY_DIR}"
      )
    # add custom target and dependency
    # (the target name is intentionally kept short to avoid windows path too long errors)
    ADD_CUSTOM_TARGET(ACME2_CPDLL_${TARGETNAME} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}-CopiedSharedLibraries)
    ADD_DEPENDENCIES(${TARGETNAME} ACME2_CPDLL_${TARGETNAME})
  ENDIF()
ENDFUNCTION()


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

    IF("${ACME_TYPE}" STREQUAL "LIBRARY")
        IF(${ACME_NAME}_BUILD_SHARED)
            SET(ACME_TYPE "SHARED_LIBRARY")
        ELSE()
            SET(ACME_TYPE "STATIC_LIBRARY")
        ENDIF()
    ENDIF()

    SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")
    FOREACH(DEPENDENCY ${ACME_DEPENDENCIES})
        IF(TARGET ${DEPENDENCY})
            GET_TARGET_PROPERTY(OBJECTLIBS ${DEPENDENCY} ACME_OBJECTLIBRARIES)
            #MESSAGE("OBJECTLIBS are : ${OBJECTLIBS}")
            IF(OBJECTLIBS)
                SET(OBJECT_LIBRARIES_OF_DEPENDENCIES ${OBJECT_LIBRARIES_OF_DEPENDENCIES} ${OBJECTLIBS})
            ENDIF()
        ENDIF()
    ENDFOREACH()
    IF(OBJECT_LIBRARIES_OF_DEPENDENCIES)
        LIST(REMOVE_DUPLICATES OBJECT_LIBRARIES_OF_DEPENDENCIES)
    ENDIF()

    #==============================================================================================
    IF("${ACME_TYPE}" STREQUAL "STATIC_LIBRARY")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} STATIC ${TARGET_CONTENT} ${OBJECT_LIBRARIES_OF_DEPENDENCIES})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND "${ACME_ENABLE_INSTALL}")
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_STATIC_LIB} COMPONENT "${ACME_PACKAGE_NAME}-devel")
        ENDIF()
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "BINARY")
    #==============================================================================================
        ADD_EXECUTABLE(${ACME_NAME} ${TARGET_CONTENT} ${OBJECT_LIBRARIES_OF_DEPENDENCIES})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_BINARY} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()
        # When dependency chain reches a binary, reset needed object_libs that need to be linked
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "OBJECT")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} OBJECT ${TARGET_CONTENT})
        # Collect object libraries needed, when someone depends on this target later
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES ${OBJECT_LIBRARIES_OF_DEPENDENCIES} $<TARGET_OBJECTS:${ACME_NAME}>)

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "CUSTOM_TARGET")
    #==============================================================================================
        ADD_CUSTOM_TARGET(${ACME_NAME} SOURCES ${TARGET_CONTENT})

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "CUSTOM_TEST_TARGET")
    #==============================================================================================
        ADD_CUSTOM_TARGET(${ACME_NAME} SOURCES ${TARGET_CONTENT})

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "TEST")
    #==============================================================================================
        ADD_TEST(
            NAME ${ACME_NAME}_UNITTESTS
            COMMAND ./${ACME_NAME} --gtest_output=xml:TestResult_${ACME_NAME}.xml
		)

        ADD_EXECUTABLE(${ACME_NAME} ${TARGET_CONTENT} ${OBJECT_LIBRARIES_OF_DEPENDENCIES} )
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_TEST} COMPONENT "${ACME_PACKAGE_NAME}-test")
        ENDIF()
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "SHARED_LIBRARY")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} SHARED ${TARGET_CONTENT} ${OBJECT_LIBRARIES_OF_DEPENDENCIES})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_SHARED_LIB} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")

    #==============================================================================================
    ELSEIF("${ACME_TYPE}" STREQUAL "PLUGIN")
    #==============================================================================================
        ADD_LIBRARY(${ACME_NAME} MODULE ${TARGET_CONTENT} ${OBJECT_LIBRARIES_OF_DEPENDENCIES})
        IF(NOT "${TARGET_CONTENT_SOURCE}" STREQUAL "" AND ${ACME_ENABLE_INSTALL})
            INSTALL(TARGETS ${ACME_NAME} DESTINATION ${ACME_INSTALL_PLUGIN} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()
        SET(OBJECT_LIBRARIES_OF_DEPENDENCIES "")

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
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY ACME_DEPENDENCIES   ${ACME_DEPENDENCIES})
        SET_PROPERTY(TARGET ${ACME_NAME} PROPERTY ACME_OBJECTLIBRARIES  ${OBJECT_LIBRARIES_OF_DEPENDENCIES})
    ENDIF()

    #==============================================================================================
    # internal lists
    #==============================================================================================
    SET(INCLUDE_PATH_LIST "${ACME_INCLUDE_BASE}")
    SET(SYSTEM_INCLUDE_PATH_LIST "")

ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_PUBLIC_HEADER)
#==============================================================================================
    IF (${ACME_ENABLE_INSTALL})
        FOREACH(PUBLIC_HEADER ${ACME_FILES_PUBLIC_HEADER})

            SET(FoundARelativeIncludeBaseToUse False)
            FOREACH(INCLUDE_BASE ${ACME_INCLUDE_BASE})
                FILE(GLOB INCLUDE_BASE_FULL_PATH "${INCLUDE_BASE}")

                # Check if the INCLUDE_BASE_FULL_PATH is part of the PUBLIC_HEADER path
                # then reuse the directory structure
                STRING(REPLACE "${INCLUDE_BASE_FULL_PATH}" "" PUBLIC_HEADER_REL_PATH ${PUBLIC_HEADER})
                IF (NOT "${PUBLIC_HEADER_REL_PATH}" STREQUAL "${PUBLIC_HEADER}")
                    FILE(RELATIVE_PATH PUBLIC_HEADER_REL_PATH ${INCLUDE_BASE_FULL_PATH} ${PUBLIC_HEADER})
                    GET_FILENAME_COMPONENT(PUBLIC_HEADER_PATH ${PUBLIC_HEADER_REL_PATH} PATH)
                    SET(PUBLIC_HEADER_INSTALL_PATH "include/${PUBLIC_HEADER_PATH}")
                    INSTALL(FILES ${PUBLIC_HEADER} DESTINATION ${PUBLIC_HEADER_INSTALL_PATH} COMPONENT "${ACME_PACKAGE_NAME}-devel")
                    SET(FoundARelativeIncludeBaseToUse True)
                    BREAK()
                ENDIF()
            ENDFOREACH()

            IF(NOT FoundARelativeIncludeBaseToUse)
                # No include bases were relevant just set the install path to be "include"
                SET(PUBLIC_HEADER_INSTALL_PATH "include")
                INSTALL(FILES ${PUBLIC_HEADER} DESTINATION ${PUBLIC_HEADER_INSTALL_PATH} COMPONENT "${ACME_PACKAGE_NAME}-devel")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDIF()

#==============================================================================================
IF(DEFINED ACME_FILES_RESOURCE)
#==============================================================================================
    IF(${ACME_ENABLE_INSTALL})
        IF (("${ACME_TYPE}" STREQUAL "TEST") OR ("${ACME_TYPE}" STREQUAL "CUSTOM_TEST_TARGET"))
            INSTALL(FILES ${ACME_FILES_RESOURCE} DESTINATION ${ACME_INSTALL_TEST_RESOURCE} COMPONENT "${ACME_PACKAGE_NAME}-test")
        ELSE()
            INSTALL(FILES ${ACME_FILES_RESOURCE} DESTINATION ${ACME_INSTALL_RESOURCE} COMPONENT "${ACME_PACKAGE_NAME}")
        ENDIF()
    ENDIF()

    SET(FILES_RESOURCE_BINARYDIR "")
    FOREACH(file ${ACME_FILES_RESOURCE})

        GET_FILENAME_COMPONENT(ONLY_FILENAME ${file} NAME)
        SET(TARGET_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/res/${ONLY_FILENAME}")
        add_custom_command(
            OUTPUT ${TARGET_FILEPATH}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${file}" "${TARGET_FILEPATH}"
            DEPENDS "${file}"
            COMMENT "Copying ${file} -> ${TARGET_FILEPATH}"
        )
        LIST(APPEND FILES_RESOURCE_BINARYDIR "${TARGET_FILEPATH}")
    ENDFOREACH()
# files are only copied if a target depends on them
        add_custom_target(ACME2_COPY_RESOURCES_${ACME_NAME} ALL DEPENDS
            ${FILES_RESOURCE_BINARYDIR}
            COMMENT "copy resource files to build folder (if changed)"
        )
        ADD_DEPENDENCIES(${ACME_NAME} ACME2_COPY_RESOURCES_${ACME_NAME})
ENDIF()

ADD_DEPENDENCIES_TO_TARGET("${ACME_NAME}" "${ACME_DEPENDENCIES}" FALSE)
ADD_DEPENDENCIES_TO_TARGET("${ACME_NAME}" "${ACME_DEPENDENCIES_SYSTEM}" TRUE)

# only needed on windows, linux-based os use rpath
IF (CMAKE_SYSTEM_NAME MATCHES "Windows" OR DEFINED ACME2_TEST_ENABLE_DLL_COPY)
  COLLECT_DLL_DEPENDENCIES_FOR_TARGET("${ACME_NAME}" DLL_TARGETS_FOR_THIS_TARGET)
  ADD_DLL_COPY_TARGET_TO_TARGET("${ACME_NAME}" "${DLL_TARGETS_FOR_THIS_TARGET}")
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

IF(TARGET ${ACME_NAME} AND NOT "${SYSTEM_INCLUDE_PATH_LIST}" STREQUAL "")
    LIST(REMOVE_DUPLICATES SYSTEM_INCLUDE_PATH_LIST)
    # SYSTEM and PUBLIC is used to indicate system include paths to the compiler, see
    # https://cmake.org/cmake/help/v3.0/command/target_include_directories.html
    TARGET_INCLUDE_DIRECTORIES(${ACME_NAME} SYSTEM PUBLIC "${SYSTEM_INCLUDE_PATH_LIST}")
ENDIF()
