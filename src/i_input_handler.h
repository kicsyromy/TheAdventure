#pragma once

#include "events.h"

class IInputHandler
{
public:
    virtual ~IInputHandler() = default;

public:
    virtual void on_key_pressed(const KeyPressEvent &event)    = 0;
    virtual void on_key_released(const KeyReleaseEvent &event) = 0;
};