#include "hero.h"
#include "codes.h"
#include "game.h"

#include <resources.h>

enum SpriteSet
{
    IdleDown,
    IdleRight,
    IdleUp,
    RunningDown,
    RunningRight,
    RunningUp,
    AttackingDown,
    AttackingRight,
    AttackingUp,
    Death
};

static constexpr std::int32_t MAX_FRAMES    = 6;
static constexpr std::int32_t ATTACK_FRAMES = 4;

Hero::Hero(Renderer &renderer, Sound &sound)
  : m_sprite{ resource_player, resource_player_size, renderer }
{
    m_sprite.scale_x() *= 2.F;
    m_sprite.scale_y() *= 2.F;
    m_sprite.set_sprite_set(SpriteSet::IdleDown);
    m_sprite.set_total_frames(MAX_FRAMES);
    m_sprite.set_frame_time(std::chrono::milliseconds{ 100 });

    m_attack_sound_id =
        sound.load_sample(resource__07_human_atk_sword_1, resource__07_human_atk_sword_1_size);
}

void Hero::attack(Sound &sound)
{
    m_is_attacking = true;

    switch (m_orientation)
    {
    case Orientation::Up: {
        m_sprite.set_sprite_set(SpriteSet::AttackingUp);
        break;
    }
    case Orientation::Down: {
        m_sprite.set_sprite_set(SpriteSet::AttackingDown);
        break;
    }
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

void Hero::update(Game &game, float attenuation)
{
    if (m_is_attacking && m_sprite.current_frame() == ATTACK_FRAMES - 1)
    {
        m_is_attacking = false;
    }

    m_is_moving = false;

    if (game.is_key_pressed(KeyCode::Up))
    {
        m_sprite.y() -= 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningUp);
        m_orientation = Orientation::Up;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Down))
    {
        m_sprite.y() += 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningDown);
        m_orientation = Orientation::Down;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Left))
    {
        m_sprite.x() -= 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningRight, true);
        m_orientation = Orientation::Left;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Right))
    {
        m_sprite.x() += 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningRight);
        m_orientation = Orientation::Right;

        m_is_moving = true;
    }

    if (!m_is_moving && !m_is_attacking)
    {
        // Go to idle
        switch (m_orientation)
        {
        case Orientation::Up: {
            m_sprite.set_sprite_set(SpriteSet::IdleUp);
            break;
        }
        case Orientation::Down: {
            m_sprite.set_sprite_set(SpriteSet::IdleDown);
            break;
        }
        case Orientation::Left: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight, true);
            break;
        }
        case Orientation::Right: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight);
            break;
        }
        }

        m_sprite.reset();
    }
}

void Hero::render(Renderer &renderer)
{
    m_sprite.render(renderer);
}

void Hero::handle_input()
{}
