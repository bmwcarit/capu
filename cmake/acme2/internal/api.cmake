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

SET(ACME2_API

    # Project metadata
    NAME
    TYPE
    DESCRIPTION
    URL
    COMPANY
    EMAIL

    # version
    VERSION_MAJOR
    VERSION_MINOR
    VERSION_PATCH
    VERSION_STRING

    # source paths
    INCLUDE_BASE
    FILES_PUBLIC_HEADER
    FILES_PRIVATE_HEADER
    FILES_SOURCE
    FILES_UNIT_TEST
    FILES_DOCUMENTATION
    FILES_RESOURCE
    FILES_GENERATED

    # dependencies
    DEPENDENCIES
    DEPENDENCIES_HEADER
    DEPENDENCIES_UNIT_TEST
    DEPENDENCIES_SYSTEM

    # install module: boolean
    ENABLE_INSTALL

    # install paths
    INSTALL_HEADER
    INSTALL_BINARY
    INSTALL_TEST
    INSTALL_TEST_RESOURCE
    INSTALL_STATIC_LIB
    INSTALL_SHARED_LIB
    INSTALL_PLUGIN
    INSTALL_RESOURCE
    INSTALL_DOCUMENTATION
    INSTALL_CMAKEFINDMODULE
    INSTALL_PKGCONFIG

    # content
    CONTENT
    CONTENT_TEST

)
