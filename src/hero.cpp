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

static constexpr std::int32_t MAX_FRAMES            = 6;
static constexpr std::int32_t ATTACK_FRAMES         = 4;
static constexpr Rect         DEFAULT_COLLISION_BOX = { 15, 30, 19, 14 };

Hero::Hero(Renderer &renderer, Sound &sound)
  : m_sprite{ resource_player, resource_player_size, renderer }
  , m_sound{ sound }
{
    scale_x() *= 2.F;
    scale_y() *= 2.F;

    width()  = m_sprite.width() / MAX_FRAMES;
    height() = m_sprite.height() / MAX_FRAMES;

    set_collision_box(DEFAULT_COLLISION_BOX);

    m_sprite.set_sprite_set(SpriteSet::IdleDown);
    m_sprite.set_total_frames(MAX_FRAMES);
    m_sprite.set_frame_time(std::chrono::milliseconds{ 100 });

    m_attack_sound_id =
        sound.load_sample(resource__07_human_atk_sword_1, resource__07_human_atk_sword_1_size);
}

void Hero::attack()
{
    m_is_attacking = true;

    switch (m_orientation)
    {
    case Orientation::Up: {
        m_sprite.set_sprite_set(SpriteSet::AttackingUp);

        auto cb = DEFAULT_COLLISION_BOX;
        cb.y -= 10;
        cb.height += 10;
        set_collision_box(cb);

        break;
    }
    case Orientation::Down: {
        m_sprite.set_sprite_set(SpriteSet::AttackingDown);

        auto cb = DEFAULT_COLLISION_BOX;
        cb.height += 5;
        set_collision_box(cb);

        break;
    }
    case Orientation::Left: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight, true);

        auto cb = DEFAULT_COLLISION_BOX;
        cb.x -= 9;
        cb.width += 9;
        set_collision_box(cb);

        break;
    }
    case Orientation::Right: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight);

        auto cb = DEFAULT_COLLISION_BOX;
        cb.width += 9;
        set_collision_box(cb);

        break;
    }
    }

    m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - ATTACK_FRAMES);
    m_sprite.reset();
    m_sound.play_sample(m_attack_sound_id);
}

bool Hero::is_attacking() const
{
    return m_is_attacking && m_sprite.current_frame() >= ATTACK_FRAMES / 2;
}

float Hero::attack_power() const
{
    return 1.F;
}

void Hero::update(Game &game, float attenuation)
{
    if (m_is_attacking && m_sprite.current_frame() == ATTACK_FRAMES - 1)
    {
        m_is_attacking = false;
        set_collision_box(DEFAULT_COLLISION_BOX);
    }

    m_is_moving = false;

    if (game.is_key_pressed(KeyCode::Up))
    {
        y() -= 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningUp);
        m_orientation = Orientation::Up;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Down))
    {
        y() += 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningDown);
        m_orientation = Orientation::Down;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Left))
    {
        x() -= 120.F * attenuation;
        m_sprite.set_sprite_set(SpriteSet::RunningRight, true);
        m_orientation = Orientation::Left;

        m_is_moving = true;
    }

    if (game.is_key_pressed(KeyCode::Right))
    {
        x() += 120.F * attenuation;
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
    m_sprite.x() = x();
    m_sprite.y() = y();

    m_sprite.scale_x() = scale_x();
    m_sprite.scale_y() = scale_y();

    m_sprite.render(renderer);
}

void Hero::on_key_pressed(const KeyPressEvent &event)
{
    if (event.key_code == KeyCode::Space)
    {
        attack();
    }
}

void Hero::on_key_released(const KeyReleaseEvent &event)
{}

float Hero::c_x() const
{
    return IThing::x();
}
float Hero::c_y() const
{
    return IThing::y();
}
float Hero::c_scale_x() const
{
    return IThing::scale_x();
}
float Hero::c_scale_y() const
{
    return IThing::scale_y();
}
