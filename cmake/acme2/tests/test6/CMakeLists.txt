DEFINE_TEST("exe->static->object")

ACME_MODULE(
    NAME                    test6_object
    TYPE                    OBJECT
    
    FILES_SOURCE            test6_object/test6_object.cpp
    INCLUDE_BASE            test6_object/include
    FILES_PUBLIC_HEADER     test6_object/include/*.h
)

ACME_MODULE(
    NAME                    test6_static
    TYPE                    STATIC_LIBRARY
    
    FILES_SOURCE            test6_static/test6_static.cpp
    INCLUDE_BASE            test6_static/include
    FILES_PUBLIC_HEADER     test6_static/include/*.h
    
    DEPENDENCIES            test6_object
)

ACME_MODULE(
    NAME                    test6_exe
    TYPE                    BINARY
    
    FILES_SOURCE            test6_executable/test6_exe.cpp
    INCLUDE_BASE            test6_executable/include
    FILES_PUBLIC_HEADER     test6_executable/include/*.h
    
    DEPENDENCIES            test6_static
)

ASSERT_TARGET_EXISTS(test6_static "target for static library must exist")
ASSERT_TARGET_EXISTS(test6_object "target for object library must exist")
ASSERT_TARGET_EXISTS(test6_exe "target for executable must exist")

# object library
ASSERT_INCLUDES_CONTAIN(     test6_object "test6_object/include" "object library must have its own include directories")
ASSERT_INCLUDES_DONT_CONTAIN(test6_object "test6_static" "object library must not have static library includes")
ASSERT_INCLUDES_DONT_CONTAIN(test6_object "test6_executable" "object library must not have executables includes")

# static library
ASSERT_INCLUDES_CONTAIN(     test6_static "test6_static/include" "static library must have its own include directories")
ASSERT_INCLUDES_CONTAIN(     test6_static "test6_object/include" "static library must have object library includes")
ASSERT_INCLUDES_DONT_CONTAIN(test6_static "test6_executable/include" "static library must not have exe includes")
ASSERT_SOURCES_CONTAIN(      test6_static "<TARGET_OBJECTS:test6_object>" "static library must have object files in SOURCES property")

#executable
ASSERT_INCLUDES_CONTAIN(test6_exe "test6_executable/include" "executable must have its own include-dirs")
ASSERT_INCLUDES_CONTAIN(test6_exe "test6_object/include" "executable must have object library include-dirs")
ASSERT_INCLUDES_CONTAIN(test6_exe "test6_static/include" "executable must have static library include-dirs")
ASSERT_LINK_LIBRARIES_CONTAIN(test6_exe "test6_static" "executable must link static library automatically")
ASSERT_LINK_LIBRARIES_DONT_CONTAIN(test6_exe "test6_object" "executable must not link object library")
ASSERT_SOURCES_DONT_CONTAIN(test6_exe "<TARGET_OBJECTS:test6_object>" "executable must not have object files in SOURCES property")

END_TEST()