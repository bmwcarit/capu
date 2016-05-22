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
SET(ACMEPLUGIN_DEPENDENCY_GRAPH   0  CACHE BOOL     "Enable dependency graph plugin")

#--------------------------------------------------------------------------
# Plugin Hooks
#--------------------------------------------------------------------------
IF(ACMEPLUGIN_DEPENDENCY_GRAPH)

IF(NOT DOXYGEN_FOUND)
	INCLUDE(FindDoxygen)
	SET(DOXYGEN_FOUND ${DOXYGEN_FOUND} CACHE INTERNAL "")
	SET(DOXYGEN_DOT_FOUND ${DOXYGEN_DOT_FOUND} CACHE INTERNAL "")
ENDIF()

IF("${DOXYGEN_DOT_EXECUTABLE}" STREQUAL "DOXYGEN_DOT_EXECUTABLE-NOTFOUND")
        ACME_WARNING("The executable of the additional software \"dot\" was not found!")
        ACME_WARNING("If you have installed \"dot\" then add the path to the executable of \"dot\" into the Cache Variable \"DOXYGEN_DOT_EXECUTABLE\".")
        ACME_WARNING("Otherwise ACME will not create the dependency graph!")
ELSE()
	ADD_PLUGIN_ON_PROJECTBUILD(ACMEPLUGIN_DEPENDENCY_GRAPH_MAKEGRAPH)
ENDIF()

ENDIF()

#--------------------------------------------------------------------------
# Plugin Functions
#--------------------------------------------------------------------------

FUNCTION(ACMEPLUGIN_DEPENDENCY_GRAPH_MAKEGRAPH)
	INCLUDE(FindDoxygen)
		add_custom_command(OUTPUT depGraphDOTFile
                   COMMAND ${CMAKE_COMMAND} --graphviz=${CMAKE_BINARY_DIR}/dependencyGraph/dependencyGraph.dot ${CMAKE_SOURCE_DIR}
				   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  
                   )
				   
	add_custom_command(OUTPUT depGraphPNG
                   COMMAND ${DOXYGEN_DOT_EXECUTABLE} -Tpng dependencyGraph/dependencyGraph.dot -o dependencyGraph.png
				   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  
                   DEPENDS depGraphDOTFile
                   )		   
				   
   add_custom_command(OUTPUT openDependencyGraph
                   COMMAND ${CMAKE_BINARY_DIR}/dependencyGraph.png
                   DEPENDS depGraphPNG
                   )
add_custom_target(MakeDependencyGraph DEPENDS openDependencyGraph)

ENDFUNCTION(ACMEPLUGIN_DEPENDENCY_GRAPH_MAKEGRAPH)

