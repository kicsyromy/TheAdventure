set (SDL2_VERSION 2.28.5)

include (FetchContent)
FetchContent_Declare (
    SDL2-${SDL2_VERSION}
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-${SDL2_VERSION}.tar.gz
    URL_HASH SHA256=9f0556e4a24ef5b267010038ad9e9948b62f236d5bcc4b22179f95ef62d84023
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

set (SDL_CMAKE_DEBUG_POSTFIX "" CACHE STRING "Remove suffix for Debug builds" FORCE)

add_library(
    ${PROJECT_ID}_sdl2 INTERFACE
)

target_include_directories (
    ${PROJECT_ID}_sdl2 SYSTEM INTERFACE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_compile_definitions (
    ${PROJECT_ID}_sdl2 INTERFACE
    -DUSING_GENERATED_CONFIG_H
    -DSDL_MAIN_HANDLED
)

if (NOT BUILD_SHARED_LIBS)
    set (SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "Don't build shared libraries unless explicitly requested" FORCE)

    if (CMAKE_POSITION_INDEPENDENT_CODE)
        set (SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code" FORCE)
    endif ()
endif ()

set (SDL2_SRC_DIR ${FETCHCONTENT_BASE_DIR}/sdl2-${SDL2_VERSION}-src)
set (SDL2_BIN_DIR ${FETCHCONTENT_BASE_DIR}/sdl2-${SDL2_VERSION}-build)

if (NOT SDL2-${SDL2_VERSION}_POPULATED)
    FetchContent_Populate (SDL2-${SDL2_VERSION})
    if (NOT BUILD_SHARED_LIBS)
        add_subdirectory (${SDL2_SRC_DIR} ${SDL2_BIN_DIR} EXCLUDE_FROM_ALL)
    else ()
        add_subdirectory (${SDL2_SRC_DIR} ${SDL2_BIN_DIR})
    endif ()
endif ()

if (NOT BUILD_SHARED_LIBS)
    set (SDL2_TARGET_NAME "SDL2-static")
    # Unless this is set SDL2 defines DLL entry point that messes with MSVC when
    # linking into a shared library
    target_compile_definitions (${SDL2_TARGET_NAME} PRIVATE -DSDL_STATIC_LIB)
else ()
    set (SDL2_TARGET_NAME "SDL2")
endif ()

execute_process (
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${FETCHCONTENT_BASE_DIR}/sdl2-${SDL2_VERSION}-src/include
        ${CMAKE_CURRENT_BINARY_DIR}/SDL2
)

target_link_libraries (
    ${PROJECT_ID}_sdl2 INTERFACE
    ${SDL2_TARGET_NAME}
)

# If the wayland-client library is available on the system then SDL with will use it but will not set it as a
# dependency so try and find it and if found add it as a link library.
find_library(
    WAYLAND_CLIENT_LIBRARY
    NAMES wayland-client libwayland-client
)

if (WAYLAND_CLIENT_LIBRARY)
    target_link_libraries (
        ${PROJECT_ID}_sdl2 INTERFACE
        ${WAYLAND_CLIENT_LIBRARY}
    )
endif ()

set (SDL2_CMAKE_CONFIG_GENERATED "${FETCHCONTENT_BASE_DIR}/sdl2-${SDL2_VERSION}-build/include/SDL_config.h")
set (SDL2_CMAKE_CONFIG_SDL_SOURCE_INCLUDE "${FETCHCONTENT_BASE_DIR}/sdl2-${SDL2_VERSION}-src/include")
set (SDL2_GENERATED_INCLUDE_SRC "#include <${SDL2_CMAKE_CONFIG_GENERATED}>\n#include <${SDL2_CMAKE_CONFIG_SDL_SOURCE_INCLUDE}/SDL.h>\n")
file (WRITE "${CMAKE_CURRENT_BINARY_DIR}/SDL2/SDL.h" "${SDL2_GENERATED_INCLUDE_SRC}")

add_library (SDL::SDL2 ALIAS ${PROJECT_ID}_sdl2)
