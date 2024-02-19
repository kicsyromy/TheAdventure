#pragma once

class IDestroyable
{
public:
    virtual ~IDestroyable() = default;

public:
    virtual void take_damage(float damage) = 0;
    virtual bool should_be_destroyed()     = 0;
};
