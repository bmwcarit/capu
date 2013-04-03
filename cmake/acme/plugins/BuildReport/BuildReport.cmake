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
# Plugin API
#--------------------------------------------------------------------------

SET(CONFIG_CREATE_BUILD_REPORT  0	CACHE BOOL 	   "enable the creation of the build report")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

MESSAGE(VERBOSE "...requires external tool Doxygen")
IF(CONFIG_CREATE_BUILD_REPORT)
	IF(NOT DOXYGEN_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindDoxygen.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_CREATE_BUILD_REPORT)
	ADD_PLUGIN_FINALIZE_HOOK(INTERNAL_CREATE_BUILD_REPORT)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_CREATE_BUILD_REPORT)
	IF(CONFIG_CREATE_BUILD_REPORT)
		IF(NOT "${DOXYGEN_EXECUTABLE}" STREQUAL "DOXYGEN_EXECUTABLE-NOTFOUND")
			MESSAGE(VERBOSE "Creating build report...")
			IF(EXISTS ${DOXYGEN_EXECUTABLE})
				SET(path_to_build_report "${ACME_PATH}/plugins/BuildReport")
				SET(path_to_doc "${CMAKE_DOC_OUTPUT_DIRECTORY}")
				SET(doxygen_run_directory "${PROJECT_SOURCE_DIR}/modules")
			
				CONFIGURE_FILE(${path_to_build_report}/doxyfile_build_report_template ${path_to_doc}/doxyfile_build_report_${PROJECT_NAME})
				CONFIGURE_FILE(${path_to_build_report}/0_mainpagetemplate.dox ${path_to_doc}/0_mainpage.dox)
				CONFIGURE_FILE(${path_to_build_report}/1_full_dependency_graph_template.dox ${path_to_doc}/1_full_dependency_graph.dox)
				
				EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${doxygen_run_directory}
							ARGS ${path_to_doc}/doxyfile_build_report_${PROJECT_NAME}
							)
				IF(EXISTS ${path_to_doc}/dependency_graph.map)
					FILE(READ ${path_to_doc}/dependency_graph.map map_content)
					FILE(APPEND ${path_to_doc}/html/_full_01dependency_01graph.html \n${map_content})			
					
					SET(highlight_content \n<script\ type="text/javascript"\ src="jquery.maphilight.js"></script>\n<script\ type="text/javascript">$(function()\ {\n$('.map').maphilight();\n});\n</script>)
					FILE(APPEND ${path_to_doc}/html/_full_01dependency_01graph.html ${highlight_content})
				
					CONFIGURE_FILE(${path_to_doc}/dependency_graph.png ${path_to_doc}/html/dependency_graph.png COPYONLY)
					CONFIGURE_FILE(${path_to_build_report}/jquery.maphilight.js ${path_to_doc}/html/jquery.maphilight.js COPYONLY)
				ENDIF()
				
				MESSAGE(VERBOSE "Build report successfully created!")
			ELSE()
				MESSAGE(WARNING "The doxygen executable ${DOXYGEN_EXECUTABLE} doesn't exist.")
				MESSAGE(WARNING "The build report was not created.")
			ENDIF()
		ENDIF()	
	ENDIF()	
ENDFUNCTION(INTERNAL_CREATE_BUILD_REPORT)