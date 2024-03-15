#include "sprite.h"

#include <cmath>

Sprite::Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer)
  : m_width{ 0 }
  , m_height{ 0 }
  , m_image_id{ renderer.load_image(data, size, &m_width, &m_height) }
{}

void Sprite::render(Renderer &renderer)
{
    renderer.draw_image(
        m_image_id,
        0,
        0,
        static_cast<std::int32_t>(std::round(m_pos_x + static_cast<float>(m_width) / 2.F)),
        static_cast<std::int32_t>(std::round(m_pos_y + static_cast<float>(m_height) / 2.F)),
        false,
        false,
        m_width,
        m_height,
        m_width,
        m_height);
}

float &Sprite::x()
{
    return m_pos_x;
}

float Sprite::x() const
{
    return m_pos_x;
}

float &Sprite::y()
{
    return m_pos_y;
}

float Sprite::y() const
{
    return m_pos_y;
}

float Sprite::width() const
{
    return static_cast<float>(m_width);
}

float Sprite::height() const
{
    return static_cast<float>(m_height);
}
