#pragma once

class IAttacker
{
public:
    virtual ~IAttacker() = default;

public:
    virtual bool is_attacking() const = 0;
};