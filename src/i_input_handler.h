#pragma once

#include "i_thing.h"

class IInputHandler
{
public:
    virtual void handle_input() = 0;
};