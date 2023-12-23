cmake_minimum_required (VERSION 3.21)

set (_CPT_PROJECT_NAME "The Adventure")
string (MAKE_C_IDENTIFIER "${_CPT_PROJECT_NAME}" PROJECT_ID)
string (TOLOWER ${PROJECT_ID} PROJECT_ID)

set (BUILD_SHARED_LIBS OFF)

project (${PROJECT_ID})
set (PROJECT_NAME "${_CPT_PROJECT_NAME}")

include (cmake/EmbedResources.cmake)

add_subdirectory (3rdparty)
add_subdirectory (src)
add_subdirectory (res)

