DEFINE_TEST("exe->object->static")

ACME_MODULE(
    NAME                    test5_static
    TYPE                    STATIC_LIBRARY
    
    FILES_SOURCE            test5_static/test5_static_static.cpp
    INCLUDE_BASE            test5_static/include
    FILES_PUBLIC_HEADER     test5_static/include/*.h
)

ACME_MODULE(
    NAME                    test5_executable_object
    TYPE                    OBJECT
    
    FILES_SOURCE            test5_object/test5_object_object.cpp
    INCLUDE_BASE            test5_object/include
    FILES_PUBLIC_HEADER     test5_object/include/*.h
    
    DEPENDENCIES            test5_static
)

ACME_MODULE(
    NAME                    test5_exe
    TYPE                    BINARY
    
    FILES_SOURCE            test5_executable/test5_exe.cpp
    INCLUDE_BASE            test5_executable/include
    FILES_PUBLIC_HEADER     test5_executable/include/*.h
    
    DEPENDENCIES            test5_executable_object
)

ASSERT_TARGET_EXISTS(test5_static "target for static library must exist")
ASSERT_TARGET_EXISTS(test5_executable_object "target for object library must exist")
ASSERT_TARGET_EXISTS(test5_exe "target for executable must exist")

# static library
ASSERT_INCLUDES_CONTAIN(test5_static "test5_static/include" "static library must have its own include directories")
ASSERT_INCLUDES_DONT_CONTAIN(test5_static "test5_object" "static library must not have object library includes")
ASSERT_INCLUDES_DONT_CONTAIN(test5_static "test5_executable" "static library must not have executables includes")

# object library
ASSERT_INCLUDES_CONTAIN(test5_executable_object "test5_object/include" "object library must have its own include directories")
ASSERT_INCLUDES_CONTAIN(test5_executable_object "test5_static" "object library must have static library includes")
ASSERT_INCLUDES_DONT_CONTAIN(test5_executable_object "test5_executable" "object library must not have executables includes")

#executable
ASSERT_INCLUDES_CONTAIN(test5_exe "test5_executable/include" "executable must have its own include-dirs")
ASSERT_INCLUDES_CONTAIN(test5_exe "test5_object/include" "executable must have object library include-dirs")
ASSERT_INCLUDES_CONTAIN(test5_exe "test5_static/include" "executable must have static library include-dirs")
ASSERT_LINK_LIBRARIES_CONTAIN(test5_exe "test5_static" "executable must link static library automatically")
ASSERT_LINK_LIBRARIES_DONT_CONTAIN(test5_exe "test5_executable_object" "executable must not link object library")
ASSERT_SOURCES_CONTAIN(test5_exe "<TARGET_OBJECTS:test5_executable_object>" "executable must have object files in SOURCES property")

END_TEST()