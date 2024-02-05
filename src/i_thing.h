#pragma once

#include <cstdint>

class IThing
{
public:
    virtual ~IThing() = default;

public:
    std::int32_t       &x();
    const std::int32_t &x() const;
    std::int32_t       &y();
    const std::int32_t &y() const;

private:
    std::int32_t m_x;
    std::int32_t m_y;
};
