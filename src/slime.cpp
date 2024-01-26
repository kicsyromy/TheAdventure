#include "slime.h"
#include "codes.h"
#include "game.h"

#include <resources.h>

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
{
    m_sprite.scale_x() *= 2.F;
    m_sprite.scale_y() *= 2.F;
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

    m_is_moving = false;

    if (game.is_key_pressed(KeyCode::W))
    {
        m_sprite.y() -= 120.F * event.seconds_elapsed;
        m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
        m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::S))
    {
        m_sprite.y() += 120.F * event.seconds_elapsed;
        m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
        m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::A))
    {
        m_sprite.x() -= 120.F * event.seconds_elapsed;
        m_sprite.set_sprite_set(SpriteSet::JumpingRight, true);
        m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
        m_orientation = Orientation::Left;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::D))
    {
        m_sprite.x() += 120.F * event.seconds_elapsed;
        m_sprite.set_sprite_set(SpriteSet::JumpingRight);
        m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
        m_orientation = Orientation::Right;

        m_is_moving = true;
    }

    if (!m_is_moving && !m_is_attacking)
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
}
