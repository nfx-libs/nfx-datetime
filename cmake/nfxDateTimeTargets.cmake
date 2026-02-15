#==============================================================================
# nfx-datetime - CMake targets
#==============================================================================

#----------------------------------------------
# Targets definition
#----------------------------------------------

# --- Create shared library if requested ---
if(NFX_DATETIME_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

# --- Create static library if requested ---
if(NFX_DATETIME_BUILD_STATIC)
    add_library(${PROJECT_NAME}-static STATIC)
    target_sources(${PROJECT_NAME}-static
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME}-static PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}-static
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::static ALIAS ${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Targets properties
#----------------------------------------------

function(configure_target target_name)
    # --- Include directories ---
    target_include_directories(${target_name}
        PUBLIC
            $<BUILD_INTERFACE:${NFX_DATETIME_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${NFX_DATETIME_SOURCE_DIR}
    )

    # --- Link libraries ---
    target_link_libraries(${target_name}
        PRIVATE
            $<BUILD_INTERFACE:nfx-stringbuilder-static>
    )

    # --- Properties ---
    set_target_properties(${target_name}
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            DEBUG_POSTFIX "-d"
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            POSITION_INDEPENDENT_CODE ON
    )

    # --- CPU optimizations (Release/RelWithDebInfo only) ---
    if(NFX_DATETIME_ENABLE_SIMD)
        target_compile_options(${target_name}
            PRIVATE
                $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:/arch:AVX2>
                $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:-march=native>
        )
    endif()

    # --- Compiler warnings ---
    target_compile_options(${target_name}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Werror>
    )
endfunction()

# --- Apply configuration to both targets ---
if(NFX_DATETIME_BUILD_SHARED)
    configure_target(${PROJECT_NAME})
    if(WIN32)
        set_target_properties(${PROJECT_NAME}
            PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS TRUE
        )

        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/nfxDateTimeVersion.rc.in
            ${CMAKE_BINARY_DIR}/nfxDateTime.rc
            @ONLY
        )
        target_sources(${PROJECT_NAME}
            PRIVATE
                ${CMAKE_BINARY_DIR}/nfxDateTime.rc)
    endif()
endif()

if(NFX_DATETIME_BUILD_STATIC)
    configure_target(${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Build configuration summary
#----------------------------------------------

if(NFX_DATETIME_ENABLE_SIMD)
    message(STATUS "nfx-datetime: Native CPU optimizations enabled (Release/RelWithDebInfo builds)")
else()
    message(STATUS "nfx-datetime: Native CPU optimizations disabled (suitable for WebAssembly)")
endif()
