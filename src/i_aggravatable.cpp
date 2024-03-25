#include "i_aggravatable.h"

#include <iostream>

class CollidableThing : public ICollidable
{
public:
    CollidableThing(float x, float y)
      : m_x{ x }
      , m_y{ y }
    {}

    float c_x() const override
    {
        return m_x;
    }
    float c_y() const override
    {
        return m_y;
    }

private:
    float m_x;
    float m_y;
};

void IAggravatable::set_aggro_area(Rect aggro_area)
{
    m_aggravation_zone.set_collision_box(aggro_area);
}

void IAggravatable::set_aggravated_by(IThing &thing)
{
    m_aggravated_by = &thing;
}

bool IAggravatable::has_aggro()
{
    auto collidable_thing = CollidableThing{ m_aggravated_by->x(), m_aggravated_by->y() };
    collidable_thing.set_collision_box({ 0, 0, 1, 1 });

    std::cerr << "TargetX: " << m_aggravated_by->x() << "\nTargetY: " << m_aggravated_by->y()
              << "\nCollX: " << m_aggravation_zone.c_x() << "\nCollY: " << m_aggravation_zone.c_y()
              << '\n';

    return m_aggravation_zone.is_colliding(collidable_thing);
}

void IAggravatable::render_aggro_area(Renderer &renderer)
{
    m_aggravation_zone.render_collision_box(renderer, has_aggro());
}
