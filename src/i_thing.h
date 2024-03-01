#pragma once

#include <cstdint>

class Game;

class IThing
{
public:
    IThing();
    virtual ~IThing() = default;

public:
    virtual void update(Game &game, float attenuation = 1.F) = 0;

public:
    std::int32_t id() const;

    float       &x();
    const float &x() const;

    float       &y();
    const float &y() const;

    float       &width();
    const float &width() const;

    float       &height();
    const float &height() const;

private:
    std::int32_t m_id{ 0 };

    float m_x{ 0.F };
    float m_y{ 0.F };

    float m_width{ 0.F };
    float m_height{ 0.F };
};
