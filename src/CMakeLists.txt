add_executable (${PROJECT_ID})

set_property (TARGET ${PROJECT_ID} PROPERTY CXX_STANDARD 17)
set_property (TARGET ${PROJECT_ID} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property (TARGET ${PROJECT_ID} PROPERTY CXX_EXTENSIONS OFF)

target_sources(
    ${PROJECT_ID}
    PRIVATE
        events.h
        game.h
        game.cpp
        main.cpp
        renderer.h
        renderer.cpp
)

target_link_libraries (
    ${PROJECT_ID}
    PRIVATE
        fmt::fmt
        SDL::SDL2
        stb::stb
)

target_compile_definitions (
    ${PROJECT_ID}
    PRIVATE
        -DPROJECT_NAME="${PROJECT_NAME}"
)