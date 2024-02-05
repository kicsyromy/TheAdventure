#include "i_thing.h"

std::int32_t &IThing::x()
{
    return m_x;
}

const std::int32_t &IThing::x() const
{
    return m_x;
}

std::int32_t &IThing::y()
{
    return m_y;
}

const std::int32_t &IThing::y() const
{
    return m_y;
}