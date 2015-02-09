# use package config to resolve dependencies,

find_package(PkgConfig QUIET)

IF(PKG_CONFIG_FOUND)
    pkg_check_modules(automotive-dlt automotive-dlt)
    IF(${automotive-dlt_FOUND})
        SET(automotive-dlt_BUILD_CONFIG 1)
    ELSE()
        SET(automotive-dlt_BUILD_CONFIG 0)
    ENDIF()
    MARK_AS_ADVANCED(
        automotive-dlt_FOUND
        automotive-dlt_INCLUDE_DIRS
        automotive-dlt_LIBRARIES
        automotive-dlt_BUILD_CONFIG
    )
ELSE()
    SET(automotive-dlt_FOUND 0)
    SET(automotive-dlt_BUILD_CONFIG 0)
ENDIF()
