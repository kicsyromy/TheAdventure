#pragma once

// Adjective
// aggravatable (comparative more aggravatable, superlative most aggravatable)
//    Capable of being aggravated.

#include "i_collidable.h"
#include "i_thing.h"

#include <functional>

class AggravationZone : public ICollidable
{
public:
    AggravationZone(std::function<float()> &&x, std::function<float()> &&y)
      : m_x{ std::move(x) }
      , m_y{ std::move(y) }
    {}

public:
    float c_x() const override
    {
        return m_x();
    }

    float c_y() const override
    {
        return m_y();
    }

private:
    std::function<float()> m_x;
    std::function<float()> m_y;
};

class IAggravatable
{
public:
    virtual ~IAggravatable() = default;

public:
    virtual float a_x() const = 0;
    virtual float a_y() const = 0;

    void set_aggro_area(Rect aggro_area);
    void set_aggravated_by(IThing &thing);
    bool has_aggro();

    void render_aggro_area(Renderer &renderer);

private:
    IThing *m_aggravated_by{ nullptr };

    AggravationZone m_aggravation_zone{ [this] { return a_x(); },
                                        [this] {
                                            return a_y();
                                        } };
};