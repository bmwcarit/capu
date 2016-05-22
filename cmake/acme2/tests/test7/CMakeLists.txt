DEFINE_TEST("exe->shared->object->object->static")

ACME_MODULE(
    NAME                    test7_static_static
    TYPE                    STATIC_LIBRARY
    
    FILES_SOURCE            test7_static/test7_static.cpp
    INCLUDE_BASE            test7_static/include
    FILES_PUBLIC_HEADER     test7_static/include/*.h
)

ACME_MODULE(
    NAME                    test7_static_objectB
    TYPE                    OBJECT
    
    FILES_SOURCE            test7_objectB/test7_objectB.cpp
    INCLUDE_BASE            test7_objectB/include
    FILES_PUBLIC_HEADER     test7_objectB/include/*.h
    
    DEPENDENCIES            test7_static_static
)

ACME_MODULE(
    NAME                    test7_static_objectA
    TYPE                    OBJECT
    
    FILES_SOURCE            test7_objectA/test7_objectA.cpp
    INCLUDE_BASE            test7_objectA/include
    FILES_PUBLIC_HEADER     test7_objectA/include/*.h
    
    DEPENDENCIES            test7_static_objectB
)

ACME_MODULE(
    NAME                    test7_shared
    TYPE                    SHARED_LIBRARY
    
    FILES_SOURCE            test7_shared/test7_shared.cpp
    INCLUDE_BASE            test7_shared/include
    FILES_PUBLIC_HEADER     test7_shared/include/*.h
    
    DEPENDENCIES            test7_static_objectA
)

ACME_MODULE(
    NAME                    test7_exe
    TYPE                    BINARY
    
    FILES_SOURCE            test7_exe/test7_exe.cpp
    INCLUDE_BASE            test7_exe/include
    FILES_PUBLIC_HEADER     test7_exe/include/*.h
    
    DEPENDENCIES            test7_shared
)

ASSERT_TARGET_EXISTS(test7_static_static "static library target must exist")
ASSERT_TARGET_EXISTS(test7_static_objectB "static library target must exist")
ASSERT_TARGET_EXISTS(test7_static_objectA "static library target must exist")
ASSERT_TARGET_EXISTS(test7_exe "static library target must exist")

# static lib
ASSERT_INCLUDES_CONTAIN(test7_static_static "test7_static/include" "static library must have its own include-dir")
ASSERT_SOURCES_CONTAIN(test7_static_static "test7_static/test7_static.cpp" "static library must build its source file")

# shared library
ASSERT_SOURCES_CONTAIN(test7_shared "<TARGET_OBJECTS:test7_static_objectA>" "shared library has object files of A in SOURCES")
ASSERT_SOURCES_CONTAIN(test7_shared "<TARGET_OBJECTS:test7_static_objectB>" "shared library has object files of B in SOURCES")
ASSERT_SOURCES_CONTAIN(test7_shared "test7_shared/test7_shared.cpp" "shared library must build its source file")

# executable
ASSERT_INCLUDES_CONTAIN(test7_exe "test7_objectA/include" "Target must have include-dirs of object library A")
ASSERT_INCLUDES_CONTAIN(test7_exe "test7_objectB/include" "Target must have include-dirs of object library B")
ASSERT_INCLUDES_CONTAIN(test7_exe "test7_shared/include" "Target must have include-dirs of shared library")
ASSERT_INCLUDES_CONTAIN(test7_exe "test7_exe/include" "Target must have its own include-dirs")

ASSERT_SOURCES_CONTAIN(test7_exe "test7_exe.cpp" "exe must build its source file")

ASSERT_LINK_LIBRARIES_CONTAIN(test7_exe "test7_shared" "did not contain link library")
ASSERT_LINK_LIBRARIES_DONT_CONTAIN(test7_exe "test7_static" "executable does not link static library")
ASSERT_LINK_LIBRARIES_DONT_CONTAIN(test7_exe "test7_static_objectA" "executable does not link object library A")
ASSERT_LINK_LIBRARIES_DONT_CONTAIN(test7_exe "test7_static_objectB" "executable does not link object library B")

END_TEST()
