include (FetchContent)

FetchContent_Declare (
    STB
    URL https://github.com/nothings/stb/archive/0bc88af4de5fb022db643c2d8e549a0927749354.tar.gz
    URL_HASH SHA256=0e05332f08c3c45405fda367ecd290558f3d8493bc9f536cc53e0e00ebe98898
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

set (STB_SRC_DIR ${FETCHCONTENT_BASE_DIR}/stb-src)

add_library(
    ${PROJECT_ID}_stb INTERFACE
)

target_include_directories (
    ${PROJECT_ID}_stb SYSTEM INTERFACE
    "${STB_SRC_DIR}"
)

target_compile_definitions (
    ${PROJECT_ID}_stb INTERFACE
    -DSTBI_NO_HDR
    -DSTBI_WINDOWS_UTF8
)

if (NOT STB_POPULATED)
    FetchContent_Populate (STB)
endif ()

add_library (stb::stb ALIAS ${PROJECT_ID}_stb)
