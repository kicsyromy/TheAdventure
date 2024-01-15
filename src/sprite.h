#ifndef SPRITE_H
#define SPRITE_H

#include "renderer.h"

#include <cstdint>

class Sprite
{
public:
    Sprite() = default;
    Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer);

    void render(Renderer &renderer);

    float &x();
    float &y();

    float &scale_x();
    float &scale_y();

private:
    float m_pos_x{ 0 };
    float m_pos_y{ 0 };

    float m_scale_x{ 1 };
    float m_scale_y{ 1 };

    std::int32_t m_width;
    std::int32_t m_height;

    std::int32_t m_image_id;
};

#endif // SPRITE_H
