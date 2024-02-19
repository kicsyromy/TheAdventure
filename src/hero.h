#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_input_handler.h"
#include "i_renderable.h"
#include "sound.h"

class Game;

class Hero
  : public IRenderable
  , public IInputHandler
  , public ICollidable
  , public IAttacker
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

    void attack();
    bool is_attacking() const override;

    void update(Game &game, float attenuation = 1.F) override;
    void render(Renderer &renderer) override;
    void on_key_pressed(const KeyPressEvent &event) override;
    void on_key_released(const KeyReleaseEvent &event) override;

private:
    AnimatedSprite m_sprite;
    Orientation    m_orientation{ Orientation::Down };
    bool           m_is_attacking{ false };
    bool           m_is_moving{ false };
    std::int32_t   m_attack_sound_id{ -1 };

    Sound &m_sound;
};