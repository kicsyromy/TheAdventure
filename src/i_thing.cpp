#include "i_thing.h"

static std::int32_t s_next_id{ 0 };

IThing::IThing()
  : m_id{ s_next_id++ }
{}

std::int32_t IThing::id() const
{
    return m_id;
}

float &IThing::x()
{
    return m_x;
}

const float &IThing::x() const
{
    return m_x;
}

float &IThing::width()
{
    return m_width;
}

const float &IThing::width() const
{
    return m_width;
}

float &IThing::height()
{
    return m_height;
}

const float &IThing::height() const
{
    return m_height;
}

float &IThing::y()
{
    return m_y;
}

const float &IThing::y() const
{
    return m_y;
}

float &IThing::scale_x()
{
    return m_scale_x;
}

const float &IThing::scale_x() const
{
    return m_scale_x;
}

float &IThing::scale_y()
{
    return m_scale_y;
}

const float &IThing::scale_y() const
{
    return m_scale_y;
}
