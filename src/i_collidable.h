#pragma once

#include "i_thing.h"
#include "renderer.h"

struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

class ICollidable : public IThing
{
public:
    virtual Rect &get_collision_box() const;
    virtual void  set_collision_box(Rect collision_box);

    virtual void render_collision_box(Renderer &renderer, bool is_colliding = false);

    virtual bool is_colliding(const ICollidable &other) const;

private:
    Rect m_collision_box;
};