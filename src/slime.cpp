#include "slime.h"
#include "codes.h"
#include "game.h"

#include <resources.h>

#include <iostream>
#include <random>

enum SpriteSet
{
    IdleRight,
    JumpingRight,
    AttackingRight,
    TakingDamageRight,
    Death
};

static constexpr std::int32_t MAX_FRAMES    = 7;
static constexpr std::int32_t IDLE_FRAMES   = 4;
static constexpr std::int32_t JUMP_FRAMES   = 6;
static constexpr std::int32_t ATTACK_FRAMES = 7;
static constexpr std::int32_t DAMAGE_FRAMES = 3;
static constexpr std::int32_t DEATH_FRAMES  = 5;

Slime::Slime(Renderer &renderer, Sound &sound)
  : m_sprite{ resource_slime, resource_slime_size, renderer }
  , m_generator{ std::random_device{}() }
  , m_distribution{ 0, 10 }
{
    m_sprite.set_collision_box({ 14, 13, 19, 18 });
    m_sprite.scale_x() *= 2.F;
    m_sprite.scale_y() *= 2.F;
    m_sprite.x() = 100.F;
    m_sprite.y() = 100.F;
    m_sprite.set_sprite_set(SpriteSet::IdleRight);
    m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - IDLE_FRAMES);
    m_sprite.set_frame_time(std::chrono::milliseconds{ 100 });

    m_attack_sound_id =
        sound.load_sample(resource__07_human_atk_sword_1, resource__07_human_atk_sword_1_size);
}

void Slime::attack(Sound &sound)
{
    m_is_attacking = true;

    switch (m_orientation)
    {
    case Orientation::Left: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight, true);
        break;
    }
    case Orientation::Right: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight);
        break;
    }
    }

    m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - ATTACK_FRAMES);
    m_sprite.reset();
    sound.play_sample(m_attack_sound_id);
}

void Slime::update(Game &game, const RenderEvent &event)
{
    if (m_is_attacking && m_sprite.current_frame() == ATTACK_FRAMES - 1)
    {
        m_is_attacking = false;
    }

    const auto now = std::chrono::steady_clock::now();
    if (now - m_last_gen_time >= std::chrono::seconds{ 1 })
    {
        m_last_gen_time = now;

        switch (m_distribution(m_generator))
        {
        case 0: {
            m_direction = Direction::Up;
            m_is_moving = true;
            break;
        }
        case 1: {
            m_direction = Direction::Down;
            m_is_moving = true;
            break;
        }
        case 2: {
            m_direction = Direction::Left;
            m_is_moving = true;
            break;
        }
        case 3: {
            m_direction = Direction::Right;
            m_is_moving = true;
            break;
        }
        default:
            m_is_moving = false;
            break;
        }
    }

    if (m_is_moving)
    {
        switch (m_direction)
        {
        case Direction::Up: {
            m_sprite.y() -= 120.F * event.seconds_elapsed;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

            m_is_moving = true;
            break;
        }
        case Direction::Down: {
            m_sprite.y() += 120.F * event.seconds_elapsed;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

            m_is_moving = true;
            break;
        }
        case Direction::Left: {
            m_sprite.x() -= 120.F * event.seconds_elapsed;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, true);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
            m_orientation = Orientation::Left;

            m_is_moving = true;
            break;
        }
        case Direction::Right: {
            m_sprite.x() += 120.F * event.seconds_elapsed;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
            m_orientation = Orientation::Right;

            m_is_moving = true;
            break;
        }
        }
    }
    else if (!m_is_attacking)
    {
        // Go to idle
        switch (m_orientation)
        {
        case Orientation::Left: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight, true);
            break;
        }
        case Orientation::Right: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight);
            break;
        }
        }

        if (m_sprite.total_frames() != IDLE_FRAMES)
        {
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - IDLE_FRAMES);
            m_sprite.reset();
        }
    }
}

void Slime::render(Renderer &renderer)
{
    m_sprite.render(renderer);
    m_sprite.render_collision_box(renderer);
}
