CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../")
FIND_PACKAGE(ACME2)

MACRO(TEST_MSG MSG DETAILS)
    MESSAGE("${TAB_SPACE} ${MSG}: ${DETAILS}")
ENDMACRO()

MACRO(DEFINE_TEST DESCRIPTION)
    MESSAGE("[----------]")
    MESSAGE("[ RUN      ] ${DESCRIPTION}")
    SET(TAB_SPACE "${TAB_SPACE}  ")
ENDMACRO()

MACRO(END_TEST)
    MESSAGE("[       OK ] ${DESCRIPTION}")
ENDMACRO()

MACRO(EXPECT_STR_CONTAINS THESTRING SUBSTRING)
    IF("${THESTRING}" MATCHES "${SUBSTRING}")
    ELSE()
        MESSAGE(FATAL_ERROR  "does not contain")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_INCLUDES_CONTAIN THETARGET INCLUDETOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(INC_DIRS "${THETARGET}" INCLUDE_DIRECTORIES)
    IF("${INC_DIRS}" MATCHES "${INCLUDETOLOOKFOR}")
    ELSE()
        TEST_MSG(${ERRORMSG} "(Include was contained)")
        MESSAGE(FATAL_ERROR "")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_INCLUDES_DONT_CONTAIN THETARGET INCLUDETOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(INC_DIRS "${THETARGET}" INCLUDE_DIRECTORIES)
    IF("${INC_DIRS}" MATCHES "${INCLUDETOLOOKFOR}")
        TEST_MSG(${ERRORMSG} "(Include was not contained)")
        MESSAGE(FATAL_ERROR "")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_SOURCES_CONTAIN THETARGET SOURCETOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(SRCS "${THETARGET}" SOURCES)
    IF (NOT POLICY CMP0051 AND "${SOURCETOLOOKFOR}" MATCHES "<TARGET_OBJECTS:")
        MESSAGE("Skipping TARGET_OBJECTS test because cmake ${CMAKE_VERSION} does not support CMP0051")
    ELSE()
      IF("${SRCS}" MATCHES "${SOURCETOLOOKFOR}")
      ELSE()
        MESSAGE("sources were: ${SRCS}")
        TEST_MSG("${ERRORMSG} (source was not contained)")
        MESSAGE(FATAL_ERROR "")
      ENDIF()
    ENDIF()
ENDMACRO()

MACRO(ASSERT_SOURCES_DONT_CONTAIN THETARGET SOURCETOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(SRCS "${THETARGET}" SOURCES)
    IF("${SRCS}" MATCHES "${SOURCETOLOOKFOR}")
        MESSAGE("sources were: ${SRCS}")
        TEST_MSG("${ERRORMSG} (source was contained)")
        MESSAGE(FATAL_ERROR "")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_LINK_LIBRARIES_CONTAIN THETARGET LIBTOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(LIBS "${THETARGET}" LINK_LIBRARIES)
    IF("${LIBS}" MATCHES "${LIBTOLOOKFOR}")
    ELSE()
        MESSAGE("libs were: ${LIBS}")
        TEST_MSG(${ERRORMSG} "(library for linking was not contained)")
        MESSAGE(FATAL_ERROR "")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_LINK_LIBRARIES_DONT_CONTAIN THETARGET LIBTOLOOKFOR ERRORMSG)
    IF (NOT TARGET ${THETARGET})
        MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
    ENDIF()
    GET_TARGET_PROPERTY(LIBS "${THETARGET}" LINK_LIBRARIES)
    IF("${LIBS}" MATCHES "${LIBTOLOOKFOR}")
        MESSAGE("libs were: ${LIBS}")
        TEST_MSG(${ERRORMSG} "(library for linking was contained)")
        MESSAGE(FATAL_ERROR "")
    ENDIF()
ENDMACRO()

MACRO(ASSERT_TARGET_EXISTS TARGETNAME ERRORMSG)
IF (NOT TARGET ${TARGETNAME})
    TEST_MSG(${ERRORMSG} "Target '${TARGETNAME}' does not exist")
    MESSAGE(FATAL_ERROR "")
ENDIF()
ENDMACRO()

MACRO(ASSERT_TARGET_NOT_EXISTS TARGETNAME ERRORMSG)
IF (TARGET ${TARGETNAME})
    TEST_MSG(${ERRORMSG} "Target '${TARGETNAME}' should not exist")
    MESSAGE(FATAL_ERROR "")
ENDIF()
ENDMACRO()

FUNCTION(ASSERT_DLLS_FOR_TARGET TARGETNAME)
  IF (NOT TARGET ${TARGETNAME})
    MESSAGE(FATAL_ERROR ERRORMSG "(was not target)" )
  ENDIF()
  COLLECT_DLL_DEPENDENCIES_FOR_TARGET(${TARGETNAME} RESULT_DLLS)
  SET(EXPECTED_DLLS ${ARGN})
  IF (EXPECTED_DLLS)
    LIST(SORT EXPECTED_DLLS)
  ENDIF()
  IF (RESULT_DLLS)
    LIST(SORT RESULT_DLLS)
  ENDIF()
  IF (NOT "${EXPECTED_DLLS}" STREQUAL "${RESULT_DLLS}")
    TEST_MSG(${ERRORMSG} "dll dependencies for ${TARGETNAME} mismatch (expected '${EXPECTED_DLLS}', got '${RESULT_DLLS}')")
    MESSAGE(FATAL_ERROR "")
  ENDIF()
ENDFUNCTION()

#test1 executable_justFromSources
#test2 executableDependingOnStaticLibrary
#test3 executableDependingOnFindScript
#test4 executableDependingOnObjectLibrary
#test5 executable_object_static
#test6 executable_static_object
#test7 exe_sharedLibrary_dependingOnChainOfObjectLibraries
#test8 executable_static_static

ACME2_PROJECT(
    #==========================================================================
    # project metadata
    #==========================================================================
    NAME                        acme2Tests
    CONTENT                     ON   test1
    CONTENT                     ON   test2
                                ON   test3
                                ON   test4
                                ON   test5
                                ON   test6
                                ON   test7
                                # ON   test8
                                ON   test9
)
