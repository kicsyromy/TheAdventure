#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "sound.h"

class Game;

class Hero
{
private:
    enum class Orientation
    {
        Up,
        Down,
        Left,
        Right
    };

public:
    Hero() = default;
    Hero(Renderer &renderer, Sound &sound);

    void attack(Sound &sound);
    void update(Game &game, const RenderEvent &event);
    void render(Renderer &renderer);

private:
    AnimatedSprite m_sprite{};
    Orientation    m_orientation{ Orientation::Down };
    bool           m_is_attacking{ false };
    bool           m_is_moving{ false };
    std::int32_t   m_attack_sound_id{ -1 };
};