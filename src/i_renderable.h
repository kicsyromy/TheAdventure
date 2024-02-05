#pragma once

#include "i_thing.h"
#include "renderer.h"

class IRenderable : public IThing
{
public:
    virtual void render(Renderer &renderer) = 0;
};