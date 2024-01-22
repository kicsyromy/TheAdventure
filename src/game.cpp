#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_hero = AnimatedSprite{ resource_player, resource_player_size, renderer };
    m_hero.scale_x() *= 2.F;
    m_hero.scale_y() *= 2.F;
    m_hero.set_sprite_set(1);
    m_hero.set_total_frames(6);
    m_hero.set_frame_time(std::chrono::milliseconds{ 100 });

    m_hero_attack_sound_id =
        m_sound.load_sample(resource__07_human_atk_sword_1, resource__07_human_atk_sword_1_size);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 127, 127, 127, 255 });
    renderer.clear();

    bool is_idle = true;

    if (is_key_pressed(KeyCode::Up))
    {
        m_hero.y() -= 120.F * event.seconds_elapsed;
        m_hero.set_sprite_set(5);
        m_hero_orientation = HeroOrientation::Up;

        is_idle = false;
    }

    if (is_key_pressed(KeyCode::Down))
    {
        m_hero.y() += 120.F * event.seconds_elapsed;
        m_hero.set_sprite_set(3);
        m_hero_orientation = HeroOrientation::Down;

        is_idle = false;
    }

    if (is_key_pressed(KeyCode::Left))
    {
        m_hero.x() -= 120.F * event.seconds_elapsed;
        m_hero.set_sprite_set(4, true);
        m_hero_orientation = HeroOrientation::Left;

        is_idle = false;
    }

    if (is_key_pressed(KeyCode::Right))
    {
        m_hero.x() += 120.F * event.seconds_elapsed;
        m_hero.set_sprite_set(4);
        m_hero_orientation = HeroOrientation::Right;

        is_idle = false;
    }

    if (is_idle && !m_hero_attacking)
    {
        switch (m_hero_orientation)
        {
        case HeroOrientation::Up: {
            m_hero.set_sprite_set(2);
            break;
        }
        case HeroOrientation::Down: {
            m_hero.set_sprite_set(0);
            break;
        }
        case HeroOrientation::Left: {
            m_hero.set_sprite_set(1, true);
            break;
        }
        case HeroOrientation::Right: {
            m_hero.set_sprite_set(1);
            break;
        }
        }
    }

    m_hero.render(renderer);

    if (m_hero_attacking)
    {
        if (m_hero.current_frame() == 3)
        {
            m_hero_attacking = false;
            m_hero.set_sprite_set(0);
            m_hero.set_total_frames(6);
        }
    }

    fps_timer += event.seconds_elapsed;
    total_seconds_elapsed += event.seconds_elapsed;
    ++frame_counter;

    if (fps_timer >= 1.0F)
    {
        fmt::print("FPS: {}\n", frame_counter);
        fps_timer     = fps_timer - 1.0F;
        frame_counter = 0;
    }
}

void Game::on_window_shown()
{}

void Game::on_window_hidden()
{}

void Game::on_window_moved(const WindowMoveEvent &event)
{}

void Game::on_window_resized(const WindowResizeEvent &event)
{}

void Game::on_window_focus_gained()
{}

void Game::on_window_focus_lost()
{}

void Game::on_window_closed()
{}

void Game::on_window_mouse_entered()
{}

void Game::on_window_mouse_left()
{}

void Game::on_mouse_moved(const MouseMoveEvent &event)
{}

void Game::on_mouse_button_pressed(const MouseButtonPressEvent &event)
{}

void Game::on_mouse_button_released(const MouseButtonReleaseEvent &event)
{}

void Game::on_mouse_wheel(const MouseWheelEvent &event)
{}

void Game::on_key_pressed(const KeyPressEvent &event)
{
    if (event.key_code == KeyCode::Space)
    {
        m_hero_attacking = true;

        switch (m_hero_orientation)
        {
        case HeroOrientation::Up: {
            m_hero.set_sprite_set(8);
            break;
        }
        case HeroOrientation::Down: {
            m_hero.set_sprite_set(6);
            break;
        }
        case HeroOrientation::Left: {
            m_hero.set_sprite_set(7, true);
            break;
        }
        case HeroOrientation::Right: {
            m_hero.set_sprite_set(7);
            break;
        }
        }

        m_hero.set_total_frames(6, 2);
        m_sound.play_sample(m_hero_attack_sound_id);
    }
}

void Game::on_key_released(const KeyReleaseEvent &event)
{}

bool Game::is_mouse_button_pressed(MouseButton button) const
{
    return m_mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(KeyCode key_code) const
{
    return m_keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}
