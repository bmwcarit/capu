############################################################################
#
# Copyright (C) 2015 BMW Car IT GmbH
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

IF(${ACME_ENABLE_INSTALL})
    SET(ACME_PDB_PATH $<TARGET_FILE_DIR:${ACME_NAME}>)
    SET(ACME_PDB_LOCATION ${ACME_PDB_PATH}/${ACME_NAME}.pdb)
    IF("${ACME_TYPE}" STREQUAL "STATIC_LIBRARY")
        SET_TARGET_PROPERTIES(${ACME_NAME} PROPERTIES COMPILE_PDB_NAME ${ACME_NAME})
        INSTALL(FILES ${ACME_PDB_LOCATION} DESTINATION ${ACME_INSTALL_STATIC_LIB} CONFIGURATIONS Debug RelWithDebInfo)
    ELSEIF("${ACME_TYPE}" STREQUAL "BINARY")
        INSTALL(FILES ${ACME_PDB_LOCATION} DESTINATION ${ACME_INSTALL_BINARY} CONFIGURATIONS Debug RelWithDebInfo)
    ELSEIF("${ACME_TYPE}" STREQUAL "SHARED_LIBRARY")
        INSTALL(FILES ${ACME_PDB_LOCATION} DESTINATION ${ACME_INSTALL_SHARED_LIB} CONFIGURATIONS Debug RelWithDebInfo)
    ELSEIF("${ACME_TYPE}" STREQUAL "PLUGIN")
        INSTALL(FILES ${ACME_PDB_LOCATION} DESTINATION ${ACME_INSTALL_PLUGIN} CONFIGURATIONS Debug RelWithDebInfo)
    ENDIF()
ENDIF()
