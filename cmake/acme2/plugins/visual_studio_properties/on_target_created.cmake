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

# this must be improved:
# it should not rely on ACME2 internal variables

CONFIGURE_FILE(${ACME2_BASE_DIR}/plugins/visual_studio_properties/Target.vcxproj.user ${CMAKE_CURRENT_BINARY_DIR})
FILE(RENAME ${CMAKE_CURRENT_BINARY_DIR}/Target.vcxproj.user ${CMAKE_CURRENT_BINARY_DIR}/${ACME_NAME}.vcxproj.user)
