#include "sprite.h"

#include <cmath>

Sprite::Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer)
  : m_width{ 0 }
  , m_height{ 0 }
  , m_image_id{ renderer.load_image(data, size, &m_width, &m_height) }
{}

void Sprite::render(Renderer &renderer)
{
    renderer.draw_image(m_image_id,
                        0,
                        0,
                        static_cast<std::int32_t>(std::round(m_pos_x)),
                        static_cast<std::int32_t>(std::round(m_pos_y)),
                        m_width,
                        m_height,
                        static_cast<std::int32_t>(std::round(m_width * m_scale_x)),
                        static_cast<std::int32_t>(std::round(m_height * m_scale_y)));
}

float &Sprite::x()
{
    return m_pos_x;
}

float &Sprite::y()
{
    return m_pos_y;
}

float &Sprite::scale_x()
{
    return m_scale_x;
}

float &Sprite::scale_y()
{
    return m_scale_y;
}