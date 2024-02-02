#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "sound.h"

#include <random>

class Game;

class Slime
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

    void          attack(Sound &sound);
    void          update(Game &game, const RenderEvent &event);
    void          render(Renderer &renderer);
    const Sprite &sprite() const;
    bool          is_colliding(const Sprite &sprite);

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

    bool m_is_colliding{ false };
};