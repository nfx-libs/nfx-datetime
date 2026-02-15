#==============================================================================
# nfx-datetime - Dependencies configuration
#==============================================================================

#----------------------------------------------
# Output configuration
#----------------------------------------------

set(_SAVED_CMAKE_REQUIRED_QUIET    ${CMAKE_REQUIRED_QUIET})
set(_SAVED_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
set(_SAVED_CMAKE_FIND_QUIETLY      ${CMAKE_FIND_QUIETLY})

set(CMAKE_REQUIRED_QUIET    ON     )
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE) # [ERROR, WARNING, NOTICE, STATUS, VERBOSE, DEBUG]
set(CMAKE_FIND_QUIETLY      ON     )

#----------------------------------------------
# Dependency versions
#----------------------------------------------

set(NFX_DATETIME_DEPS_NFX_STRINGBUILDER_VERSION "0.7.0")

#----------------------------------------------
# FetchContent dependencies
#----------------------------------------------

include(FetchContent)

if(DEFINED ENV{CI})
    set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)
else()
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
endif()
set(FETCHCONTENT_QUIET OFF)

# --- nfx-stringbuilder ---
find_package(nfx-stringbuilder QUIET)
if(NOT nfx-stringbuilder_FOUND)
    set(NFX_STRINGBUILDER_ENABLE_SIMD         ${NFX_DATETIME_ENABLE_SIMD} CACHE BOOL "")
    set(NFX_STRINGBUILDER_BUILD_STATIC        ON  CACHE BOOL "Build static library"        FORCE)
    set(NFX_STRINGBUILDER_BUILD_SHARED        OFF CACHE BOOL "Build shared library"        FORCE)
    set(NFX_STRINGBUILDER_BUILD_TESTS         OFF CACHE BOOL "Build tests"                 FORCE)
    set(NFX_STRINGBUILDER_BUILD_SAMPLES       OFF CACHE BOOL "Build samples"               FORCE)
    set(NFX_STRINGBUILDER_BUILD_BENCHMARKS    OFF CACHE BOOL "Build benchmarks"            FORCE)
    set(NFX_STRINGBUILDER_BUILD_DOCUMENTATION OFF CACHE BOOL "Build Doxygen documentation" FORCE)
    set(NFX_STRINGBUILDER_INSTALL_PROJECT     OFF CACHE BOOL "Install project"             FORCE)
    set(NFX_STRINGBUILDER_PACKAGE_SOURCE      OFF CACHE BOOL "Enable source package"       FORCE)

    FetchContent_Declare(
        nfx-stringbuilder
            GIT_REPOSITORY https://github.com/nfx-libs/nfx-stringbuilder
            GIT_TAG        ${NFX_DATETIME_DEPS_NFX_STRINGBUILDER_VERSION}
            GIT_SHALLOW    TRUE
    )
endif()

if(NOT nfx-stringbuilder_FOUND)
    FetchContent_MakeAvailable(nfx-stringbuilder)
endif()

#----------------------------------------------
# Cleanup
#----------------------------------------------

set(CMAKE_REQUIRED_QUIET    ${_SAVED_CMAKE_REQUIRED_QUIET})
set(CMAKE_MESSAGE_LOG_LEVEL ${_SAVED_CMAKE_MESSAGE_LOG_LEVEL})
set(CMAKE_FIND_QUIETLY      ${_SAVED_CMAKE_FIND_QUIETLY})
