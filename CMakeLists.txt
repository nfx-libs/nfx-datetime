#==============================================================================
# nfx-datetime - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-datetime
    VERSION 0.6.0
    DESCRIPTION "Cross-platform C++ DateTime library with 100-nanosecond precision and ISO 8601 support"
    HOMEPAGE_URL "https://github.com/nfx-libs/nfx-datetime"
    LANGUAGES CXX
)

#----------------------------------------------
# Version configuration
#----------------------------------------------

set(NFX_DATETIME_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} CACHE INTERNAL "nfx-datetime major version")
set(NFX_DATETIME_VERSION_MINOR ${PROJECT_VERSION_MINOR} CACHE INTERNAL "nfx-datetime minor version")
set(NFX_DATETIME_VERSION_PATCH ${PROJECT_VERSION_PATCH} CACHE INTERNAL "nfx-datetime patch version")
set(NFX_DATETIME_VERSION       ${PROJECT_VERSION}       CACHE INTERNAL "nfx-datetime version")

#----------------------------------------------
# Testing setup
#----------------------------------------------

enable_testing()

#----------------------------------------------
# Build options
#----------------------------------------------

# --- Library build types ---
option(NFX_DATETIME_BUILD_STATIC        "Build static library"               ON )
option(NFX_DATETIME_BUILD_SHARED        "Build shared library"               OFF)

# --- Build components ---
option(NFX_DATETIME_BUILD_TESTS         "Build tests"                        OFF)
option(NFX_DATETIME_BUILD_SAMPLES       "Build samples"                      OFF)
option(NFX_DATETIME_BUILD_BENCHMARKS    "Build benchmarks"                   OFF)
option(NFX_DATETIME_BUILD_DOCUMENTATION "Build Doxygen documentation"        OFF)

# --- Performance options ---
option(NFX_DATETIME_ENABLE_SIMD         "Enable native CPU optimizations"    ON )

# --- Installation and packaging ---
option(NFX_DATETIME_INSTALL_PROJECT     "Install project"                    OFF)
option(NFX_DATETIME_PACKAGE_SOURCE      "Enable source package generation"   OFF)

#----------------------------------------------
# CMake modules configuration
#----------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#----------------------------------------------
# Main configuration
#----------------------------------------------

include(nfxDateTimeBuildConfig)
include(nfxDateTimeDependencies)
include(nfxDateTimeSources)
include(nfxDateTimeTargets)

#----------------------------------------------
# Installation and packaging
#----------------------------------------------

include(nfxDateTimeInstall)
include(nfxDateTimePackaging)

#----------------------------------------------
# Targets
#----------------------------------------------

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmark)
add_subdirectory(doc)
