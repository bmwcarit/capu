DEFINE_TEST("Executable depending on object lib")

ACME_MODULE(
    NAME                    test4_objectLib
    TYPE                    OBJECT
    
    FILES_SOURCE            test4_objectLib/test4_objectA.cpp
    INCLUDE_BASE            test4_objectLib/test4_objectInclude
    FILES_PUBLIC_HEADER     test4_objectLib/test4_objectInclude/*.h
)

ACME_MODULE(
    NAME                    test4_exe
    TYPE                    BINARY
    
    FILES_SOURCE            test4.cpp
    
    DEPENDENCIES            test4_objectLib
)

ASSERT_TARGET_EXISTS(test4_objectLib "target for object library must exist")
ASSERT_TARGET_EXISTS(test4_exe "target for executablemust exist")

ASSERT_INCLUDES_CONTAIN(test4_exe "test4_objectInclude" "executable must get include dirs from object library")
ASSERT_SOURCES_CONTAIN(test4_exe "<TARGET_OBJECTS:test4_objectLib>" "executable must get object files for its SOURCES property")

END_TEST()
