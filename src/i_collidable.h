#pragma once

#include "renderer.h"

struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

class ICollidable
{
public:
    virtual ~ICollidable() = default;

public:
    const Rect &get_collision_box() const;
    void        set_collision_box(Rect collision_box);

    virtual void render_collision_box(Renderer &renderer, bool is_colliding = false);
    virtual bool is_colliding(const ICollidable &other);

protected:
    virtual float c_x() const = 0;
    virtual float c_y() const = 0;

private:
    Rect m_collision_box;
};