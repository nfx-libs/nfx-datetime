#==============================================================================
# nfx-datetime - CMake Sources
#==============================================================================

#----------------------------------------------
# Source files
#----------------------------------------------

set(private_sources)

list(APPEND private_sources
    ${NFX_DATETIME_SOURCE_DIR}/DateTime.cpp
    ${NFX_DATETIME_SOURCE_DIR}/DateTimeOffset.cpp
    ${NFX_DATETIME_SOURCE_DIR}/TimeSpan.cpp
)
