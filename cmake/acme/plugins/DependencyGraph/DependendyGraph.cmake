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

SET(CONFIG_CREATE_DEPENDENCY_GRAPH	 0	CACHE BOOL	   "enable the creation of a dependency graph")


#--------------------------------------------------------------------------
# Plugin External Tools
#--------------------------------------------------------------------------

MESSAGE(VERBOSE "...requires external tool Dot")
IF(CONFIG_CREATE_DEPENDENCY_GRAPH)
	IF(NOT DOXYGEN_DOT_FOUND)
		INCLUDE(${ACME_PATH}/tools/FindDot.cmake)
	ENDIF()
ENDIF()


#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------

IF(CONFIG_CREATE_DEPENDENCY_GRAPH)
	ADD_PLUGIN_DEPENDENCY_HOOK(INTERNAL_ADD_DEPENDENCY_EDGES\(\"\${ad_name}\"\))
	ADD_PLUGIN_FINALIZE_HOOK(INTERNAL_CREATE_DEPENDENCY_GRAPH)
ENDIF()


#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(INTERNAL_CREATE_DEPENDENCY_GRAPH)
	IF(CONFIG_CREATE_DEPENDENCY_GRAPH)
		IF(NOT "${DOXYGEN_DOT_EXECUTABLE}" STREQUAL "DOXYGEN_DOT_EXECUTABLE-NOTFOUND")
			IF(EXISTS ${DOXYGEN_DOT_EXECUTABLE})
				MESSAGE("Creating dependency graph...")
				FILE(WRITE ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot "digraph ${PROJECT_NAME} { \n")
				FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot label="Dependency graph of ${PROJECT_NAME}"\;\n bgcolor=gray95\; \n)
				
				INTERNAL_DOT_ADD_NODE("${GLOBAL_UTILS_MODULES_STATIC}"	 "filled"	"gold"		"record")
				INTERNAL_DOT_ADD_NODE("${GLOBAL_UTILS_MODULES_DYNAMIC}"	 "filled"	"cyan3"		"record")
				INTERNAL_DOT_ADD_NODE("${GLOBAL_UTILS_MODULES_EXE}"		 "filled"	"red"		"record")
				INTERNAL_DOT_ADD_NODE("${GLOBAL_PACKAGE_LIBRARIES}"		 "dotted"	"yellow"	"record")				
				FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot "subgraph clustercmake_project_modules { \n")
				FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot label="Added Cmake Projects and External Libraries"\;\n labelloc="b"\;\n fontcolor=grey20\;\n style=dotted\;\n color=grey20\;\n)
				INTERNAL_DOT_ADD_CMAKE_PROJECT("${GLOBAL_CMAKE_PROJECTS}")
				FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot "}\n")
		
				INTERNAL_DOT_ADD_EDGE("${GLOBAL_DEPENDENCY_EDGES}" "")
		
				FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot "}")
		
				EXEC_PROGRAM("${DOXYGEN_DOT_EXECUTABLE}"
						ARGS "-Tpng \"${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot\" -o\"${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.png\""		
						OUTPUT_VARIABLE dot_run_out
						RETURN_VALUE dot_run_ret)
						
				IF (NOT "${dot_run_ret}" STREQUAL "0") 
					MESSAGE(WARNING "ERROR: PNG GENERATION FAILED with code ${dot_run_ret}")
					MESSAGE(WARNING "${dot_run_out}")
				ELSE()
					MESSAGE(VERBOSE "PNG generation successfull.")
				ENDIF()	
			
				EXEC_PROGRAM("${DOXYGEN_DOT_EXECUTABLE}"
						ARGS "-Tcmapx \"${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot\" -o\"${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.map\""		
						OUTPUT_VARIABLE dot_run_out
						RETURN_VALUE dot_run_ret)

				IF (NOT "${dot_run_ret}" STREQUAL "0") 
					MESSAGE(WARNING "ERROR: MAP GENERATION FAILED with code ${dot_run_ret}")
					MESSAGE(WARNING "${dot_run_out}")
				ELSE()
					MESSAGE(VERBOSE "MAP generation successfull.")
				ENDIF()	
				
			ELSE()
				MESSAGE(WARNING "The dot executable ${DOXYGEN_DOT_EXECUTABLE} doesn't exist.")
			ENDIF()
		ENDIF()	
	ENDIF()
ENDFUNCTION(INTERNAL_CREATE_DEPENDENCY_GRAPH)


FUNCTION(INTERNAL_ADD_DEPENDENCY_EDGES dependency)
		SET(${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES ${${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES} ${dependency} CACHE INTERNAL "")
		LIST(REMOVE_DUPLICATES ${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES)
		SET(${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES ${${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES} CACHE INTERNAL "")
		SET(GLOBAL_DEPENDENCY_EDGES ${GLOBAL_DEPENDENCY_EDGES} ${CURRENT_MODULE_NAME}_DEPENDENCY_EDGES CACHE INTERNAL "global list of all dependencies that are used to generate the dependency graph") 
		LIST(REMOVE_DUPLICATES GLOBAL_DEPENDENCY_EDGES)
		SET(GLOBAL_DEPENDENCY_EDGES ${GLOBAL_DEPENDENCY_EDGES} CACHE INTERNAL "global list of all dependencies that are used to generate the dependency graph") 
ENDFUNCTION(INTERNAL_ADD_DEPENDENCY_EDGES)


FUNCTION(INTERNAL_DOT_ADD_NODE list_of_nodes style_type color shape_type)	
	FOREACH(entry ${list_of_nodes})
		SET(url "file:///${PROJECT_SOURCE_DIR}/modules/${entry}")
		IF("${${entry}_TEST_FILES}" STREQUAL "")
			FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot \"${entry}\"\ [shape=${shape_type},\ style=${style_type},\ fillcolor=${color},\ URL=\"${url}\"]; \n)
		ELSE()
			FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot \"${entry}\"\ [shape=${shape_type},\ label="{<f0> ${entry} | <f1> ${entry}Test}",\ style=${style_type},\ fillcolor=${color},\ URL=\"${url}\"]; \n)
		ENDIF()
	ENDFOREACH()
ENDFUNCTION(INTERNAL_DOT_ADD_NODE)


FUNCTION(INTERNAL_DOT_ADD_EDGE global_list edge_type)
	FOREACH(global_list_entry ${global_list})
		STRING(REPLACE "_DEPENDENCY_EDGES" "" name_list_entry "${global_list_entry}")
		FOREACH(dependency ${${global_list_entry}})
			FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot "\"${name_list_entry}\" -> \"${dependency}\"; \n")
		ENDFOREACH()
	ENDFOREACH()
ENDFUNCTION(INTERNAL_DOT_ADD_EDGE)


FUNCTION(INTERNAL_DOT_ADD_CMAKE_PROJECT global_list)
	FOREACH(entry ${global_list})
		SET(url "file:///${PROJECT_SOURCE_DIR}/deliverable/${entry}_${PROJECT_NAME}/doc/html/_full_01dependency_01graph.html")
		FILE(APPEND ${CMAKE_DOC_OUTPUT_DIRECTORY}/dependency_graph.dot \"${entry}\"\ [shape=diamond,\ style=filled,\ fillcolor=grey,\ URL=\"${url}\"]\; \n)
	ENDFOREACH()
ENDFUNCTION(INTERNAL_DOT_ADD_CMAKE_PROJECT)
