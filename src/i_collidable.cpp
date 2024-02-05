#include "i_collidable.h"

const Rect &ICollidable::get_collision_box() const
{
    return m_collision_box;
}

void ICollidable::set_collision_box(Rect collision_box)
{
    m_collision_box = collision_box;
}

void ICollidable::render_collision_box(Renderer &renderer, bool is_colliding)
{
    if (!is_colliding)
    {
        renderer.set_color({ 0, 0, 255, 255 });
    }
    else
    {
        renderer.set_color({ 255, 0, 0, 255 });
    }
    renderer.draw_rect(static_cast<std::int32_t>(std::round(x() + m_collision_box.x * scale_x())),
                       static_cast<std::int32_t>(std::round(y() + m_collision_box.y * scale_y())),
                       m_collision_box.width * scale_x(),
                       m_collision_box.height * scale_y());

    if (!is_colliding)
    {
        renderer.set_color({ 0, 0, 255, 64 });
    }
    else
    {
        renderer.set_color({ 255, 0, 0, 64 });
    }
    renderer.fill_rect(static_cast<std::int32_t>(std::round(x() + m_collision_box.x * scale_x())),
                       static_cast<std::int32_t>(std::round(y() + m_collision_box.y * scale_y())),
                       m_collision_box.width * scale_x(),
                       m_collision_box.height * scale_y());
}

bool ICollidable::is_colliding(const ICollidable &other) const
{
    auto r1 = Rect{ x() + m_collision_box.x * scale_x(),
                    y() + m_collision_box.y * scale_y(),
                    m_collision_box.width * scale_x(),
                    m_collision_box.height * scale_y() };

    auto r2 = Rect{ other.x() + other.m_collision_box.x * other.scale_x(),
                    other.y() + other.m_collision_box.y * other.scale_y(),
                    other.m_collision_box.width * other.scale_x(),
                    other.m_collision_box.height * other.scale_y() };

    return r1.x < r2.x + r2.width && r1.x + r1.width > r2.x && r1.y < r2.y + r2.height &&
           r1.y + r1.height > r2.y;
}
