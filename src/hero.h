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
    Hero(Renderer &renderer, Sound &sound);

    void          attack(Sound &sound);
    void          update(Game &game, const RenderEvent &event);
    void          render(Renderer &renderer);
    const Sprite &sprite() const;
    bool          is_colliding(const Sprite &sprite);

private:
    AnimatedSprite m_sprite;
    Orientation    m_orientation{ Orientation::Down };
    bool           m_is_attacking{ false };
    bool           m_is_moving{ false };
    std::int32_t   m_attack_sound_id{ -1 };

    bool m_is_colliding{ false };
};