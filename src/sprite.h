#pragma once

#include "renderer.h"

#include <cstdint>

class Sprite
{
public:
    Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer);

    virtual ~Sprite() = default;

    // Copy constructor
    // ex: Sprite s1;
    //     Sprite s2{ s1 };
    Sprite(const Sprite &) = default;
    // Move constructor
    // ex: Sprite s1;
    //     Sprite s2{ std::move(s1) };
    Sprite(Sprite &&) = default;
    // Copy assignment operator
    // ex: Sprite s1{ data, size, renderer };
    //     Sprite s2;
    //     s2 = s1;
    Sprite &operator=(const Sprite &) = default;
    // Move assignment operator
    // ex: Sprite s1;
    //     Sprite s2;
    //     s2 = std::move(s1);
    Sprite &operator=(Sprite &&) = default;

    virtual void render(Renderer &renderer);

    float &x();
    float &y();

    float &scale_x();
    float &scale_y();

protected:
    std::int32_t m_width;
    std::int32_t m_height;

    std::int32_t m_image_id;

private:
    float m_pos_x{ 0 };
    float m_pos_y{ 0 };

    float m_scale_x{ 1 };
    float m_scale_y{ 1 };
};
