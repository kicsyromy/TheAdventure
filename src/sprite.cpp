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
                        false,
                        false,
                        m_width,
                        m_height,
                        static_cast<std::int32_t>(std::round(m_width * m_scale_x)),
                        static_cast<std::int32_t>(std::round(m_height * m_scale_y)));
}

void Sprite::set_collision_box(Sprite::Rect collision_box)
{
    m_collision_box = collision_box;
}

void Sprite::render_collision_box(Renderer &renderer)
{
    renderer.set_color({ 0, 0, 255, 255 });
    renderer.draw_rect(static_cast<std::int32_t>(std::round(m_pos_x + m_collision_box.x)),
                       static_cast<std::int32_t>(std::round(m_pos_y + m_collision_box.y)),
                       m_collision_box.width * m_scale_x,
                       m_collision_box.height * m_scale_y);

    renderer.set_color({ 0, 0, 255, 64 });
    renderer.fill_rect(static_cast<std::int32_t>(std::round(m_pos_x + m_collision_box.x)),
                       static_cast<std::int32_t>(std::round(m_pos_y + m_collision_box.y)),
                       m_collision_box.width * m_scale_x,
                       m_collision_box.height * m_scale_y);
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
