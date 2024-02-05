#include "i_thing.h"

float &IThing::x()
{
    return m_x;
}

const float &IThing::x() const
{
    return m_x;
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