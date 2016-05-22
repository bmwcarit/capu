SET(ACME2_TEST_ENABLE_DLL_COPY TRUE) # enable test mode

DEFINE_TEST("copy dll: no dll (exe->static)")

ACME_MODULE(
    NAME                    test9a_staticA
    TYPE                    STATIC_LIBRARY

    FILES_SOURCE            test9a_staticA.cpp
)

ACME_MODULE(
    NAME                    test9a_binary_no_dll
    TYPE                    BINARY

    FILES_SOURCE            test9a_binary_no_dll.cpp
    DEPENDENCIES            test9a_staticA
)

ASSERT_TARGET_EXISTS(test9a_staticA "target for lib must exist")
ASSERT_TARGET_EXISTS(test9a_binary_no_dll "target for executable must exist")
ASSERT_TARGET_NOT_EXISTS(ACME2_CPDLL_test9a_staticA "target for copying must not exist")
ASSERT_TARGET_NOT_EXISTS(ACME2_CPDLL_test9a_binary_no_dll "target for copying must not exist")
ASSERT_DLLS_FOR_TARGET(test9a_binary_no_dll)

END_TEST()

DEFINE_TEST("copy dll: single dll (exe->dll)")

ACME_MODULE(
    NAME                    test9b_sharedA
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9b_sharedA.cpp
)

ACME_MODULE(
    NAME                    test9b_binary_single_dll
    TYPE                    BINARY

    FILES_SOURCE            test9b_binary_single_dll.cpp
    DEPENDENCIES            test9b_sharedA
)

ASSERT_TARGET_EXISTS(test9b_sharedA "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9b_binary_single_dll "target for executable must exist")
ASSERT_TARGET_EXISTS(ACME2_CPDLL_test9b_binary_single_dll "target for copying must exist")
ASSERT_TARGET_NOT_EXISTS(ACME2_CPDLL_test9b_sharedA "target for copying must not exist")
ASSERT_DLLS_FOR_TARGET(test9b_binary_single_dll "test9b_sharedA")

END_TEST()

DEFINE_TEST("copy dll: multiple dlls, multiple uses (exe->dll1 -->dll2->static->dll3)")

ACME_MODULE(
    NAME                    test9c_sharedA
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9c_sharedA.cpp
)

ACME_MODULE(
    NAME                    test9c_staticA
    TYPE                    STATIC_LIBRARY

    FILES_SOURCE            test9c_staticA.cpp
    DEPENDENCIES            test9c_sharedA
)

ACME_MODULE(
    NAME                    test9c_sharedB
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9c_sharedB.cpp
    DEPENDENCIES            test9c_staticA
)

ACME_MODULE(
    NAME                    test9c_sharedC
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9c_sharedC.cpp
)

ACME_MODULE(
    NAME                    test9c_binary_multiple_dlls
    TYPE                    BINARY

    FILES_SOURCE            test9c_binary_multiple_dlls.cpp
    DEPENDENCIES            test9c_sharedB
                            test9c_sharedC
)

ASSERT_TARGET_EXISTS(test9c_sharedA "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9c_sharedB "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9c_sharedC "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9c_staticA "target for static lib must exist")
ASSERT_TARGET_EXISTS(test9c_binary_multiple_dlls "target for executable must exist")
ASSERT_TARGET_EXISTS(ACME2_CPDLL_test9c_binary_multiple_dlls "target for copying must exist")
ASSERT_DLLS_FOR_TARGET(test9c_binary_multiple_dlls "test9c_sharedA;test9c_sharedB;test9c_sharedC")

END_TEST()


DEFINE_TEST("copy dll: multiple ntested dlls (exe->dll->dll)")

ACME_MODULE(
    NAME                    test9d_sharedA
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9d_sharedA.cpp
)

ACME_MODULE(
    NAME                    test9d_sharedB
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9d_sharedB.cpp
)

ACME_MODULE(
    NAME                    test9d_sharedA
    TYPE                    SHARED_LIBRARY

    FILES_SOURCE            test9d_sharedA.cpp
)

ACME_MODULE(
    NAME                    test9d_binary_nested_dlls
    TYPE                    BINARY

    FILES_SOURCE            test9d_binary_nested.cpp
    DEPENDENCIES            test9d_sharedA
                            test9d_sharedB
)

ASSERT_TARGET_EXISTS(test9d_sharedA "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9d_sharedB "target for shared lib must exist")
ASSERT_TARGET_EXISTS(test9d_binary_nested_dlls "target for executable must exist")
ASSERT_TARGET_EXISTS(ACME2_CPDLL_test9d_binary_nested_dlls "target for copying must exist")
ASSERT_DLLS_FOR_TARGET(test9d_binary_nested_dlls "test9d_sharedA;test9d_sharedB")

END_TEST()
