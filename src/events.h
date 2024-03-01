#pragma once

#include "codes.h"

#include <cstdint>
#include <cstring>

class Renderer;

struct KeyPressEvent final
{
    KeyCode key_code;
};

struct KeyReleaseEvent final
{
    KeyCode key_code;
};

struct MouseMoveEvent final
{
    std::int32_t x;
    std::int32_t y;
};

struct MouseButtonPressEvent final
{
    std::int32_t x;
    std::int32_t y;
    MouseButton  button;
};

struct MouseButtonReleaseEvent final
{
    std::int32_t x;
    std::int32_t y;
    MouseButton  button;
};

struct MouseWheelEvent final
{
    std::int32_t x;
    std::int32_t y;
    bool         flipped;
};

struct WindowMoveEvent final
{
    std::int32_t x;
    std::int32_t y;
};

struct WindowResizeEvent final
{
    std::int32_t width;
    std::int32_t height;
    Renderer    &renderer;
};

struct RenderEvent final
{
    float       seconds_elapsed;
    std::size_t frames_rendered;
};
