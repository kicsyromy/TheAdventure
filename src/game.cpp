#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_bg_music_id = m_sound.load_music(resource_world_bg, resource_world_bg_size);
    m_sound.play_music(m_bg_music_id);

    m_hero  = std::make_optional<Hero>(renderer, m_sound);
    m_slime = std::make_optional<Slime>(renderer, m_sound);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 127, 127, 127, 255 });
    renderer.clear();

    m_slime->update(*this, event);
    m_hero->update(*this, event);

    m_slime->is_colliding(m_hero->sprite());
    m_hero->is_colliding(m_slime->sprite());

    m_slime->render(renderer);
    m_hero->render(renderer);

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
        m_hero->attack(m_sound);
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
