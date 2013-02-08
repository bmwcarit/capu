#
# Copyright (C) 2012 BMW Car IT GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

# Adds a LicenseHeaderCheck target to the project, if a license_template.txt exists in the main folder.
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/license_template.txt)
		add_custom_target(LicenseHeaderCheck
        java -cp ${ACME_PATH}/plugins/LicenseHeaderChecker LicenseHeaderChecker license_template.txt ".*\\.(h|cpp|c|hpp|java)" ${CMAKE_CURRENT_SOURCE_DIR}/modules
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Checking License Headers" VERBATIM
		)
ENDIF()
