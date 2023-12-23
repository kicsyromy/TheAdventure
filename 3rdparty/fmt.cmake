include (FetchContent)

set (FMT_VERSION 10.1.1)

FetchContent_Declare (
    FMT-${FMT_VERSION}
    URL https://github.com/fmtlib/fmt/archive/refs/tags/${FMT_VERSION}.tar.gz
    URL_HASH SHA256=78b8c0a72b1c35e4443a7e308df52498252d1cefc2b08c9a97bc9ee6cfe61f8b
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set (FMT_SRC_DIR ${FETCHCONTENT_BASE_DIR}/fmt-${FMT_VERSION}-src)
set (FMT_BIN_DIR ${FETCHCONTENT_BASE_DIR}/fmt-${FMT_VERSION}-build)

set (FMT_DOC OFF)
set (FMT_INSTALL OFF)
set (FMT_OS OFF)
set (FMT_TEST OFF)

if (NOT FMT-${FMT_VERSION}_POPULATED)
    FetchContent_Populate (FMT-${FMT_VERSION})
    if (NOT BUILD_SHARED_LIBS)
        add_subdirectory (${FMT_SRC_DIR} ${FMT_BIN_DIR} EXCLUDE_FROM_ALL)
        install (
            DIRECTORY ${FMT_SRC_DIR}/include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
            FILES_MATCHING PATTERN "*.h"
        )
    else ()
        add_subdirectory (${FMT_SRC_DIR} ${FMT_BIN_DIR})
    endif ()
endif ()