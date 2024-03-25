#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "i_aggravatable.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_destroyable.h"
#include "i_renderable.h"
#include "sound.h"

#include <random>

class Game;

class Slime
  : public IThing
  , public IRenderable
  , public ICollidable
  , public IDestroyable
  , public IAttacker
  , public IAggravatable
{
private:
    enum class Orientation
    {
        Left,
        Right
    };

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

public:
    Slime(Renderer &renderer, Sound &sound);

    void attack();

    void   update(Game &game, float attenuation = 1.F) override;
    void   render(Renderer &renderer) override;
    float &render_x() override;
    float &render_y() override;

    void take_damage(float damage) override;
    bool should_be_destroyed() override;

    bool  is_attacking() const override;
    float attack_power() const override;

private:
    float c_x() const override;
    float c_y() const override;

    float a_x() const override;
    float a_y() const override;

private:
    AnimatedSprite m_sprite;
    Orientation    m_orientation{ Orientation::Left };
    Direction      m_direction{ Direction::Left };
    bool           m_is_attacking{ false };
    bool           m_is_moving{ false };
    std::int32_t   m_attack_sound_id{ -1 };

    std::chrono::time_point<std::chrono::steady_clock> m_last_gen_time{
        std::chrono::steady_clock::now()
    };
    std::mt19937                    m_generator;
    std::uniform_int_distribution<> m_distribution;

    float m_health{ 3.F };
    bool  m_should_be_destroyed{ false };

    Sound &m_sound;
};