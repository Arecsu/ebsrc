cmake_minimum_required(VERSION 3.16)
project(EarthBound-PC VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# Add pkg-config paths
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_PREFIX})
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${SDL2_MIXER_LIBRARY_DIRS})

# Include directories
include_directories(include)
include_directories(src)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.c"
)

# Exclude SNES-specific files for now
list(FILTER SOURCES EXCLUDE REGEX ".*generated.*")
list(FILTER SOURCES EXCLUDE REGEX ".*spc700.*")

# Create executable
add_executable(earthbound ${SOURCES})

# Link libraries
target_link_libraries(earthbound ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
target_include_directories(earthbound PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# Compiler flags
target_compile_options(earthbound PRIVATE ${SDL2_CFLAGS_OTHER})
target_compile_options(earthbound PRIVATE -Wall -Wextra -O2)

# Copy assets directory to build folder
add_custom_command(TARGET earthbound POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(earthbound "-framework CoreFoundation")
elseif(WIN32)
    target_link_libraries(earthbound winmm)
endif()