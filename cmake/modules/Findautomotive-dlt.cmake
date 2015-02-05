SET(automotive-dlt_FOUND 0)

# use package config to resolve dependencies,

find_package(PkgConfig QUIET)

IF(PKG_CONFIG_FOUND)
    pkg_check_modules(automotive-dlt automotive-dlt)

    MARK_AS_ADVANCED(
        automotive-dlt_FOUND
        automotive-dlt_INCLUDE_DIRS
        automotive-dlt_LIBRARIES
    )
ENDIF()
