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
# Enable message output
#--------------------------------------------------------------------------

SET(TEST_ENABLE_MESSAGES 1 CACHE INTERNAL "")

#--------------------------------------------------------------------------
# If a toolchain was provided and the toolchain file exists make sure,
# that all variables are set as early as possible.
#--------------------------------------------------------------------------

IF (NOT "${CMAKE_TOOLCHAIN_FILE}" STREQUAL "" )
	IF (EXISTS "${CMAKE_TOOLCHAIN_FILE}")
		INCLUDE(${CMAKE_TOOLCHAIN_FILE})
		MESSAGE(STATUS "Using toolchain file: " ${CMAKE_TOOLCHAIN_FILE})
	ELSE()
		MESSAGE(STATUS "WARNING: toolchain file not found: " ${CMAKE_TOOLCHAIN_FILE})
	ENDIF()
ENDIF()

#--------------------------------------------------------------------------
# Store the current path to acme build system.
# There is the macro CMAKE_CURRENT_LIST_DIR, but it requires Cmake 2.8.3 or higher.
# This solution works on CMake 2.6 as well.
#--------------------------------------------------------------------------

get_filename_component(ACME_PATH ${CMAKE_CURRENT_LIST_FILE} PATH) 
MESSAGE(STATUS "using ACME base directory: ${ACME_PATH}")

#--------------------------------------------------------------------------
# Include configurations, internal implementations and a test file
#--------------------------------------------------------------------------

INCLUDE(${ACME_PATH}/internal/config.cmake)
INCLUDE(${ACME_PATH}/internal/functions.cmake)
#INCLUDE(${ACME_PATH}/test/test.cmake)
#INCLUDE(${ACME_PATH}/test/testcases.cmake)


#--------------------------------------------------------------------------
# Functions provided by ACME
#--------------------------------------------------------------------------

##
# \page     ACME_API ACME_API
# \text 	\n
# \section  ACME_ADD_CMAKE_PROJECT ACME_ADD_CMAKE_PROJECT
# \text  	Function that uses the commands `ExternalProject_Add()` and `Add_External_Library()` to add a cmake project. \n \n
# \text		\b Signature:
# \code
# ACME_ADD_CMAKE_PROJECT( 	<project_name>
#				LIBNAMES	"<libname1>" "<libname2>" ...
#				LIBDIRS "<libdir>" ...
#				INCLUDE_DIRS "<incl_dir>" ...		 (relative from "${PROJECT_SOURCE_DIR}/build/<project_name>")
#				BINARY_INCLUDE_DIRS "<incl_dir>" ...     (relative from "${PROJECT_SOURCE_DIR}/build/<project_name>/src/<project_name>-build/"
#				ABSOLUTE_INCLUDE_DIRS "<incl_dir>" ...   (absolute path)
#				URL "<url>"
#				CHECKSUM "<url_md5>"
#				SOURCE_DIR "<project_source_dir>" 
#				CMAKE_ARGUMENTS "<argument_name>:<typ>=<value>" ...
#				DEPENDENT_DEFINITIONS "<definition1>" ...
#				DEPENDENT_DEBUG_DEFINITIONS "<definition2>" ...
#				DEPENDENT_RELEASE_DEFINITIONS "<definition3>" ...
#				INSTALL	1 | 0 (default)
#			)
# \endcode
# \param    aap_project_name name of the added cmake project
# \modifies BUILD_${aap_project_name}				\n
#			[${aap_project_name}_INCLUDE_DIR]		\n	
#			[${aap_project_name}_LIBRARIES_DIR]		\n
#			[${aap_project_name}_LIBRARIES]			\n
#			[${aap_project_name}_FOUND]				\n
#			[${aap_project_name}_INTERNAL]
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_CMAKE_PROJECT aap_project_name)
	INTERNAL_ADD_CMAKE_PROJECT("${aap_project_name}" "${ARGN}")
ENDFUNCTION(ACME_ADD_CMAKE_PROJECT)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_EXTERNAL_LIBRARY ACME_ADD_EXTERNAL_LIBRARY
# \text  	Adds an external library to the build. \n \n
# \text     \b Signature: 
# \code
# ACME_ADD_EXTERNAL_LIBRARY(  	<library_name>											
#				INCLUDE_DIRS "<include_dir1>" "<include_dir2>" ...		
#				LIBRARY_DIRS "<library_dir1>" ...						
#				LIBNAMES "<name1>" ...								
#				DEPENDENT_DEFINITIONS "<definition1>" ...				
#				DEPENDENT_DEBUG_DEFINITIONS "<definition2>" ...				
#				DEPENDENT_RELEASE_DEFINITIONS "<definition3>" ...		
#			   )
# \endcode
# \param    ael_library_name name of the added external library
# \modifies <library_name>_INCLUDE_DIR						\n
#			<library_name>_LIBRARIES_DIR					\n
#			<library_name>_LIBRARIES						\n
#			<library_name>_DEPENDENT_DEBUG_DEFINITIONS		\n
#			<library_name>_DEPENDENT_RELEASE_DEFINITIONS	\n
#			<library_name>_FOUND							\n
#			<library_name>_INTERNAL							\n
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_EXTERNAL_LIBRARY ael_library_name)
	INTERNAL_ADD_EXTERNAL_LIBRARY("${ael_library_name}" "${ARGN}")
ENDFUNCTION(ACME_ADD_EXTERNAL_LIBRARY)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_SUBDIRECTORY ACME_ADD_SUBDIRECTORY
# \text  	Sets the lokal variable \a CURRENT_MODULE_NAME, invokes the CMake\n
#           command `ADD_SUBDIRECTORY()` and the Function `INTERNAL_JUST_DOIT()`.
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_SUBDIRECTORY sub_dir_name)
	INTERNAL_ACME_ADD_SUBDIRECTORY("${sub_dir_name}")
ENDFUNCTION(ACME_ADD_SUBDIRECTORY)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_MODULE ACME_ADD_MODULE
# \text  	Sets properties of the module \a ad_module_name. The module name should be consistent with the added subdirectory \n
# \text		by the function [ACME_ADD_SUBDIRECTORY()](@ref ACME_ADD_SUBDIRECTORY).
# \param    ad_module_name name of the added module
# \modifies ${ad_module_name}_HAS_SOURCE_FILES 	\n
#			${ad_module_name}_MODULE_TYPE 		\n
# 			${ad_module_name}_DIR 				\n
#			${ad_module_name}_BUILD_ENABLED 	\n
#			${ad_module_name}_FOUND 			\n
#			WITH_${ad_module_name} 				
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_MODULE ad_module_name ad_type)
	INTERNAL_ADD_MODULE("${ad_module_name}" "${ad_type}")
ENDFUNCTION(ACME_ADD_MODULE)

FUNCTION(ACME_ADD_OPTIONAL_MODULE aom_module_name aom_type)
	INTERNAL_ADD_OPTIONAL_MODULE("${aom_module_name}" "${aom_type}")
ENDFUNCTION(ACME_ADD_OPTIONAL_MODULE)

##
# \page     ACME_API ACME_API
# \section  ACME_REQUIRED_PACKAGE ACME_REQUIRED_PACKAGE
# \text  	Looks for the file \a Find<pkg_name>.cmake in the cmake module path and adds the required include directories and \n
# 			libraries to the build. The current module name is specified by the invoking \a CMakeList.txt.
# \param    pkg_name name of the added package
# \modifies ${pkg_name}_FOUND						\n
#			${pkg_name}_INCLUDE_DIRS				\n
#			${pkg_name}_LIBRARIES					\n
#			${CURRENT_MODULE_NAME}_PACKAGE_LIBS		\n
#			[${CURRENT_MODULE_NAME}_BUILD_ENABLED]	
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_REQUIRED_PACKAGE pkg_name)
	INTERNAL_REQUIRED_PACKAGE("${pkg_name}")
ENDFUNCTION(ACME_REQUIRED_PACKAGE)

##
# \page     ACME_API ACME_API
# \section  ACME_OPTIONAL_PACKAGE ACME_OPTIONAL_PACKAGE
# \text  	Optionally searches for the file \a Find<pkg_name>.cmake in the cmake module path and adds the required include \n
#			directories and libraries to the build. The current module name is specified by the invoking \a CMakeList.txt.
# \param    pkg_name name of the optionally added package
# \modifies ${pkg_name}_FOUND						\n
#			${pkg_name}_INCLUDE_DIRS				\n
#			${pkg_name}_LIBRARIES					\n
#			${CURRENT_MODULE_NAME}_PACKAGE_LIBS		
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_OPTIONAL_PACKAGE pkg_name)
	INTERNAL_OPTIONAL_PACKAGE("${pkg_name}")
ENDFUNCTION(ACME_OPTIONAL_PACKAGE)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_DEPENDENCY ACME_ADD_DEPENDENCY
# \text  	Adds a dependency from the current module (i.e. the current \a CMakeList.txt) to \a ad_name. To add several dependencies \n
#			use several function calls with just 1 Parameter per call!
# \param    ad_name name of the module to which a dependency is created
# \modifies ${CURRENT_MODULE_NAME}_DEPENDENCIES				\n
#			[${CURRENT_MODULE_NAME}_BUILD_ENABLED]			\n
#			[${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS]	\n
#			[${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS]		
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_DEPENDENCY ad_name)
	INTERNAL_ADD_DEPENDENCY("${ad_name}")
ENDFUNCTION(ACME_ADD_DEPENDENCY)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_DEFINITION ACME_ADD_DEFINITION
# \text  	Sets a release and a debug definition for a specific module (specified by the invoking \a CMakeList.txt). These \n
#			definitions are added to the target by the function `INTERNAL_JUST_DOIT()`. To add several definitions use several \n
#			function calls with just 1 Parameter per call!
# \param    ad_definition added release and debug definition
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS \n
#			${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_DEFINITION ad_definition)
	INTERNAL_ADD_DEFINITION("${ad_definition}")
ENDFUNCTION(ACME_ADD_DEFINITION)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_DEBUG_DEFINITION ACME_ADD_DEBUG_DEFINITION
# \text  	Sets a debug definition for a specific module (specified by the invoking \a CMakeList.txt). These definitions are \n
#			added to the target by the function `INTERNAL_JUST_DOIT()`. To add several definitions use several function calls \n
#			with just 1 Parameter per call!
# \param    add_definition	added debug defintion
# \modifies ${CURRENT_MODULE_NAME}_DEBUG_DEFINITIONS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_DEBUG_DEFINITION add_definition)
	INTERNAL_ADD_DEBUG_DEFINITION("${add_definition}")
ENDFUNCTION(ACME_ADD_DEBUG_DEFINITION)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_RELEASE_DEFINITION ACME_ADD_RELEASE_DEFINITION
# \text  	Sets a release definition for a specific module (specified by the invoking \a CMakeList.txt). These definitions are \n
#			added to the target by the function `INTERNAL_JUST_DOIT()`. To add several definitions use several function calls \n
#			with just 1 Parameter per call!
# \param    ard_definition	added release defintion
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_DEFINITIONS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_RELEASE_DEFINITION ard_definition)
	INTERNAL_ADD_RELEASE_DEFINITION("${ard_definition}")
ENDFUNCTION(ACME_ADD_RELEASE_DEFINITION)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_COMPILER_FLAG ACME_ADD_COMPILER_FLAG
# \text  	Sets a release and a debug compiler flag for a specific module (specified by the invoking \a CMakeList.txt). These \n
#			compiler flags are added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several \n
#			function calls with just 1 Parameter per call!
# \param    ad_compiler_flag added release and debug compiler flag
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS \n
# 			${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_COMPILER_FLAG ad_compiler_flag)
	INTERNAL_ADD_COMPILER_FLAG("${ad_compiler_flag}")
ENDFUNCTION(ACME_ADD_COMPILER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_DEBUG_COMPILER_FLAG ACME_ADD_DEBUG_COMPILER_FLAG
# \text  	Sets a debug compiler flag for a specific module (specified by the invoking \a CMakeList.txt). These compiler flags \n
#			are added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several function calls \n
#			with just 1 Parameter per call!
# \param    adcf_compiler_flag added debug compiler flag
# \modifies ${CURRENT_MODULE_NAME}_DEBUG_COMPILER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_DEBUG_COMPILER_FLAG adcf_compiler_flag)
	INTERNAL_ADD_DEBUG_COMPILER_FLAG("${adcf_compiler_flag}")
ENDFUNCTION(ACME_ADD_DEBUG_COMPILER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_RELEASE_COMPILER_FLAG ACME_ADD_RELEASE_COMPILER_FLAG
# \text  	Sets a release compiler flag for a specific module (specified by the invoking \a CMakeList.txt). These compiler flags \n
#			are added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several function calls \n
#			with just 1 Parameter per call!
# \param    arcf_compiler_flag added release compiler flag
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_COMPILER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_RELEASE_COMPILER_FLAG arcf_compiler_flag)
	INTERNAL_ADD_RELEASE_COMPILER_FLAG("${arcf_compiler_flag}")
ENDFUNCTION(ACME_ADD_RELEASE_COMPILER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_LINKER_FLAG ACME_ADD_LINKER_FLAG
# \text  	Sets a release and a debug linker flag for a specific module (specified by the invoking \a CMakeList.txt). These \n
#			linker flags are added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several \n
#			function calls with just 1 Parameter per call!
# \param    alf_linker_flag added releasse and debug linker flag
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS \n
#			${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_LINKER_FLAG alf_linker_flag)
	INTERNAL_ADD_LINKER_FLAG("${alf_linker_flag}")
ENDFUNCTION(ACME_ADD_LINKER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_DEBUG_LINKER_FLAG ACME_ADD_DEBUG_LINKER_FLAG
# \text  	Sets a debug linker flag for a specific module (specified by the invoking \a CMakeList.txt). These linker flags are \n
#			added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several function calls with \n
#			just 1 Parameter per call!
# \param    adlf_linker_flag added debug linker flag
# \modifies ${CURRENT_MODULE_NAME}_DEBUG_LINKER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_DEBUG_LINKER_FLAG adlf_linker_flag)
	INTERNAL_ADD_DEBUG_LINKER_FLAG("${adlf_linker_flag}")
ENDFUNCTION(ACME_ADD_DEBUG_LINKER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_RELEASE_LINKER_FLAG ACME_ADD_RELEASE_LINKER_FLAG
# \text  	Sets a release linker flag for a specific module (specified by the invoking \a CMakeList.txt). These linker flags are \n
#			added to the target by the function `INTERNAL_JUST_DOIT()`. To add several flags use several function calls with \n
#			just 1 Parameter per call!
# \param    arlf_linker_flag added release linker flag
# \modifies ${CURRENT_MODULE_NAME}_RELEASE_LINKER_FLAGS
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_RELEASE_LINKER_FLAG arlf_linker_flag)
	INTERNAL_ADD_RELEASE_LINKER_FLAG("${arlf_linker_flag}")
ENDFUNCTION(ACME_ADD_RELEASE_LINKER_FLAG)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_FILE ACME_ADD_FILE
# \text  	Adds all found public header files, intern header files and modules source files with the filename given in the \n
#			<\a parameter>. It additionally adds all testfiles with the filename <\a parameter>Test. To add files use just the \n
#			filename without any file extensions. To add several different files use several function calls with just 1 \n
#			Parameter per call!
# \param    ${ARGN}	name of the added public header files, intern header files, module source files and test files
# \modifies ${CURRENT_MODULE_NAME}_HAS_SOURCE_FILES				\n
#			${CURRENT_UPPER_MODULE_NAME}_MODULE_SOURCE_FILES	\n
#			${CURRENT_MODULE_NAME}_TEST_FILES
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_FILE)
	INTERNAL_ADD_FILE(${ARGN})
ENDFUNCTION(ACME_ADD_FILE)

FUNCTION(ACME_ADD_OPTIONAL_FILE aoc_name)
	INTERNAL_ADD_OPTIONAL_FILE("${aoc_name}" "${ARGN}")
ENDFUNCTION(ACME_ADD_OPTIONAL_FILE)

##
# \page     ACME_API ACME_API
# \section  ACME_LINK_LIBRARY ACME_LINK_LIBRARY
# \text  	Adds the library \a ll_name to the adequate list, which is then linked to the target by the function \n
#			`INTERNAL_JUST_DOIT()`. The target is specified by the invoking \a CMakeList.txt. To add several libraries, you \n
#			can either use several function calls, or specify the arguments with spaces in between.
# \param    ll_name name of the added library
# \param	${ARGN} list of names of additional libraries
# \modifies ${CURRENT_MODULE_NAME}_LIBRARIES
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_LINK_LIBRARY ll_name)
	INTERNAL_LINK_LIBRARY("${ll_name}" ${ARGN})
ENDFUNCTION(ACME_LINK_LIBRARY ll_name)

##
# \page     ACME_API ACME_API
# \section  ACME_LINK_LIBRARY_GROUP ACME_LINK_LIBRARY_GROUP
# \text  	Adds the library-group (indicated with spaces between the different libraries) to the adequate list, which is \n
#			then linked to the target by the function `INTERNAL_JUST_DOIT()`. The target is specified by the invoking \n
#			\a CMakeList.txt.
# \param    llg_name name of the first library from the library group
# \param	${ARGN} names of the rest of the libraries
# \modifies ${CURRENT_MODULE_NAME}_LIBRARIES
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_LINK_LIBRARY_GROUP llg_name)
	INTERNAL_LINK_LIBRARY_GROUP("${llg_name}" ${ARGN})
ENDFUNCTION(ACME_LINK_LIBRARY_GROUP)

##
# \page     ACME_API ACME_API
# \section  ACME_ADD_INSTALL_FILE ACME_ADD_INSTALL_FILE
# \text    	Adds the files in the paramters (separated by spaces) to the list \a ${CURRENT_MODULE_NAME}_INSTALL_FILES, which is \n
#			then used to install the files into the \a "bin"-directory (in the directory \a deliverable) by the function \n
#			`INTERNAL_JUST_DOIT()`. The \a CURRENT_MODULE_NAME is specified by the invoking \a CMakeList.txt. To install several \n
#			files, you can either use several function calls, or specify the arguments with spaces in between.
# \param    ${ARGN} names of the files that should be installed
# \modifies ${CURRENT_MODULE_NAME}_INSTALL_FILES
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_ADD_INSTALL_FILE)
	INTERNAL_ADD_INSTALL_FILE("${ARGN}")
ENDFUNCTION(ACME_ADD_INSTALL_FILE)

FUNCTION(ACME_INSTALL_RESOURCES)
	INTERNAL_INSTALL_RESOURCES("${ARGN}")
ENDFUNCTION(ACME_INSTALL_RESOURCES)

##
# \page     ACME_API ACME_API
# \section  ACME_FINALIZE ACME_FINALIZE
# \text  	Function that removes duplicates from the global lists, builds unit tests (if the cache variable \n
# 			\a CONFIG_BUILD_GLOBAL_TEST_EXECUTABLE is set to "True"), creates a dependency graph (if the cache variable \n
#           \a CONFIG_CREATE_DEPENDENCY_GRAPH is "True"), creates the build report (\a CONFIG_CREATE_BUILD_REPORT has to be set \n
# 			to "True") and prints the build report. 
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_FINALIZE)
	INTERNAL_FINALIZE()
ENDFUNCTION(ACME_FINALIZE)

#--------------------------------------------------------------------------
# Prints the configuration report
#--------------------------------------------------------------------------
FUNCTION(ACME_REPORT)
	INTERNAL_REPORT()
ENDFUNCTION(ACME_REPORT)

#FUNCTION(ACME_JUST_DOIT)
#	INTERNAL_JUST_DOIT()
#ENDFUNCTION(ACME_JUST_DOIT)

#MACRO(INSTALL_MODULE)
#	INTERNAL_INSTALL_MODULE("${ARGN}")
#ENDMACRO(INSTALL_MODULE)

#--------------------------------------------------------------------------
# Functions to test the build process of ACME
#--------------------------------------------------------------------------

##
# \page     ACME_API ACME_API
# \section  ACME_PRINT_STATE ACME_PRINT_STATE
# \text  	Prints all used cmake variables, build options, added cmake projects, used file extensions, external include \n
#			directories, used modules, module types, module properties, external library properties, package properties and \n
#			test properties. Use the function without paramters.
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_PRINT_STATE)
	INTERNAL_PRINT_STATE()
ENDFUNCTION(ACME_PRINT_STATE)

##
# \page     ACME_API ACME_API
# \section  ACME_SAVE_STATE ACME_SAVE_STATE
# \text  	Saves all used cache variables in the following manner: \n
#			\a <state_name>_<name_cache_variable>, whereby \a <state_name> is the parameter of `ACME_SAVE_STATE()` and \n
#			\a <name_cache_variable> is the name of the cache variable that is to be saved.
# \param    state_name name of the saved state
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_SAVE_STATE state_name)
	INTERNAL_SAVE_STATE("${state_name}")
ENDFUNCTION(ACME_SAVE_STATE)

##
# \page     ACME_API ACME_API
# \section  ACME_DELETE_STATE ACME_DELETE_STATE
# \text  	Deletes the state mentioned in the paramter of the function call.
# \param    state_name name of the state that should be deleted
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_DELETE_STATE state_name)
	INTERNAL_DELETE_STATE("${state_name}")
ENDFUNCTION(ACME_DELETE_STATE)

##
# \page     ACME_API ACME_API
# \section  ACME_COMPARE_STATES ACME_COMPARE_STATES
# \text  	Compares \a state1 and \a state2 and prints the differences between these two states. \n
#			\a state1 and \a state2 have to be saved by the function [ACME_SAVE_STATE()](@ref ACME_SAVE_STATE) in advance.
# \param    state1 first state that ought to be compared
# \param	state2 second state that ought to be compared
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_COMPARE_STATES state1 state2)
	INTERNAL_COMPARE_STATES("${state1}" "${state2}" "${ARGN}")
ENDFUNCTION(ACME_COMPARE_STATES)

##
# \page     ACME_API ACME_API
# \section  ACME_TEST ACME_TEST
# \text  	Tests the supplied ACME scripts. Currently there are 23 testcases implemented. To test the ACME scripts you just \n
#			need to configure the \a TestProject supplied with ACME (directory \a TestProject in the ACME source directory). \n
#			The results of the testcases will be printed. Delete your cache before testing the ACME scripts.
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n
FUNCTION(ACME_TEST)
	INTERNAL_TEST()
ENDFUNCTION(ACME_TEST)


#--------------------------------------------------------------------------
# Include external plugins
#--------------------------------------------------------------------------

INCLUDE(${ACME_PATH}/internal/plugins.cmake)



#
# \page     ACME_API ACME_API
# \section  
# \text  	
# \param    
# \modifies 
# \details 	--------------------------------------------------------------------------------------------------------------------\n \n



