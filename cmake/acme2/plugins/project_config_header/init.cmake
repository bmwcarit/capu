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
SET(TARGET_DIRECTORY ${CMAKE_BINARY_DIR}/BuildConfig)

SET(EXPORTED_VARIABLES
    PROJECT_NAME
    PROJECT_TYPE
    PROJECT_DESCRIPTION
    PROJECT_COMPANY
    PROJECT_URL
    PROJECT_EMAIL
    RAMSES_VERSION
    CMAKE_BUILD_TYPE
    CMAKE_CXX_COMPILER
    CMAKE_CXX_COMPILER_ID
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_SYSTEM_NAME
    CMAKE_SYSTEM_VERSION
    CMAKE_TOOLCHAIN_FILE
    CMAKE_VERSION
    GIT_COMMIT_COUNT
    GIT_COMMIT_HASH
    TARGET_ARCH
    TARGET_COMPILER
    TARGET_OS
)

IF(NOT GIT_FOUND)
    FIND_PACKAGE(Git)
ENDIF()

IF(GIT_FOUND)
    EXEC_PROGRAM(${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS rev-list HEAD --count
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        RETURN_VALUE GIT_RETURN_VALUE
    )

    IF("${GIT_RETURN_VALUE}" STREQUAL "0")

        # yes, we have a git repository

        EXEC_PROGRAM(${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}
            ARGS rev-parse --short HEAD
            OUTPUT_VARIABLE GIT_COMMIT_HASH
        )
        EXEC_PROGRAM(${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}
            ARGS branch --remote --merged HEAD
            OUTPUT_VARIABLE GIT_COMMIT_BRANCH
        )
        STRING(REGEX REPLACE "\n" ";" GIT_COMMIT_BRANCH "${GIT_COMMIT_BRANCH}")
        FOREACH(branchname ${GIT_COMMIT_BRANCH})
            IF(NOT "${branchname}" MATCHES "HEAD")
                SET(GIT_COMMIT_BRANCH "${branchname}")
                BREAK()
            ENDIF()
        ENDFOREACH()
        STRING(STRIP "${GIT_COMMIT_BRANCH}" GIT_COMMIT_BRANCH)
    ELSE()

        # no, here's no git repository
        SET(GIT_COMMIT_COUNT "(unknown)")
        SET(GIT_COMMIT_HASH "(unknown)")
        SET(GIT_COMMIT_BRANCH "(unknown)")

    ENDIF()
ENDIF()

STRING(REPLACE "-" "_" PROJECT_NAME_NO_DASH ${PROJECT_NAME})
STRING(TOUPPER ${PROJECT_NAME_NO_DASH} PROJECT_NAME_CAPITAL)

SET(EXPORTED_FULL_STRING "")
SET(EXPORTED_ACME2_SYMBOLS "")

FOREACH(VAR ${EXPORTED_VARIABLES})
    IF(NOT DEFINED ${VAR})
        SET(${VAR} "(unknown)")
    ENDIF()
    SET(EXPORTED_ACME2_SYMBOLS "${EXPORTED_ACME2_SYMBOLS}\nconst char* const ${PROJECT_NAME_CAPITAL}_${VAR} = \"@${VAR}@\";")
    SET(EXPORTED_FULL_STRING "${EXPORTED_FULL_STRING}\n    \"${VAR} = @${VAR}@\\n\"")
ENDFOREACH()

SET(EXPORTED_FULL_STRING "${EXPORTED_FULL_STRING};")

CONFIGURE_FILE(
    ${CMAKE_CURRENT_LIST_DIR}/build-config.h.in
    ${TARGET_DIRECTORY}/${PROJECT_NAME}-build-config.h.in
)

CONFIGURE_FILE(
    ${TARGET_DIRECTORY}/${PROJECT_NAME}-build-config.h.in
    ${TARGET_DIRECTORY}/${PROJECT_NAME}-build-config.h
)

ACME_INFO("G ${PROJECT_NAME}BuildConfig.h")

# add this folder to global include list
INCLUDE_DIRECTORIES(${TARGET_DIRECTORY})
