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

MACRO(ACME_CALL_PLUGIN_HOOK name)

    STRING(TOLOWER "${name}" name_lower)
    FILE(GLOB plugins ${ACME2_PLUGIN_DIR}/*/${name_lower}*.cmake)

    FOREACH(plugin ${plugins})

        STRING(REGEX REPLACE "${ACME2_PLUGIN_DIR}/" "" plugin_file "${plugin}")
        STRING(REGEX REPLACE "//" "/" list_file "${plugin_file}")

        STRING(REGEX REPLACE "([^/]*)/.*" "\\1" plugin_name "${plugin_file}")
        STRING(REGEX REPLACE ".*/(.*)\\.cmake" "\\1" file_name "${plugin_file}")

        STRING(TOUPPER "${plugin_name}" plugin_name_upper)
        OPTION(ACME_ENABLE_PLUGIN_${plugin_name_upper} "Enable ACME 2 plugin '${plugin_name}'" OFF)

        IF(ACME_ENABLE_PLUGIN_${plugin_name_upper})
            ACME_DEBUG("plugin-hook: ${plugin_name}/${file_name}")
            INCLUDE(${plugin} OPTIONAL)
        ENDIF()

    ENDFOREACH()
ENDMACRO()

