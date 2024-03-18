#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_input_handler.h"
#include "i_renderable.h"
#include "i_thing.h"
#include "sound.h"

class Game;

class Hero
  : public IThing
  , public IRenderable
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

    void         attack();
    inline float speed() const
    {
        return m_speed;
    };

    bool  is_attacking() const override;
    float attack_power() const override;

    void   update(Game &game, float attenuation = 1.F) override;
    void   render(Renderer &renderer) override;
    float &render_x() override;
    float &render_y() override;
    void   on_key_pressed(const KeyPressEvent &event) override;
    void   on_key_released(const KeyReleaseEvent &event) override;

private:
    float c_x() const override;
    float c_y() const override;

private:
    AnimatedSprite m_sprite;
    Orientation    m_orientation{ Orientation::Down };

    float m_speed{ 80.F };

    bool         m_is_attacking{ false };
    bool         m_is_moving{ false };
    std::int32_t m_attack_sound_id{ -1 };

    Sound    &m_sound;
    Renderer &m_renderer;
};