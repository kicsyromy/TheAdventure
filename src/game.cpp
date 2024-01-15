#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

#include <iostream>

Game::~Game()
{}

void Game::load_assets(Renderer &renderer)
{
    res_hero       = renderer.load_image(resource_player, resource_player_size);
    res_tile_grass = renderer.load_image(resource_grass, resource_grass_size);

    m_rock_in_water_06 =
        Sprite{ resource_rock_in_water_06, resource_rock_in_water_06_size, renderer };
    m_rock_in_water_06.scale_x() = 4.F;
    m_rock_in_water_06.scale_y() = 4.F;
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 127, 127, 127, 255 });
    renderer.clear();

    if (is_key_pressed(KeyCode::Up))
    {
        m_rock_in_water_06.y() -= 20.F * event.seconds_elapsed;
    }

    if (is_key_pressed(KeyCode::Down))
    {
        m_rock_in_water_06.y() += 20.F * event.seconds_elapsed;
    }

    if (is_key_pressed(KeyCode::Left))
    {
        m_rock_in_water_06.x() -= 20.F * event.seconds_elapsed;
    }

    if (is_key_pressed(KeyCode::Right))
    {
        m_rock_in_water_06.x() += 20.F * event.seconds_elapsed;
    }

    m_rock_in_water_06.render(renderer);

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
{
    fmt::print("Window shown\n");
}

void Game::on_window_hidden()
{
    fmt::print("Window hidden\n");
}

void Game::on_window_moved(const WindowMoveEvent &event)
{
    fmt::print("Window moved to ({}, {})\n", event.x, event.y);
}

void Game::on_window_resized(const WindowResizeEvent &event)
{
    fmt::print("Window resized to ({}, {})\n", event.width, event.height);
}

void Game::on_window_focus_gained()
{
    fmt::print("Window focus gained\n");
}

void Game::on_window_focus_lost()
{
    fmt::print("Window focus lost\n");
}

void Game::on_window_closed()
{
    fmt::print("Window closed\n");
}

void Game::on_window_mouse_entered()
{
    fmt::print("Mouse entered window\n");
}

void Game::on_window_mouse_left()
{
    fmt::print("Mouse left window\n");
}

void Game::on_mouse_moved(const MouseMoveEvent &event)
{
    fmt::print("Mouse moved to ({}, {})\n", event.x, event.y);
}

void Game::on_mouse_button_pressed(const MouseButtonPressEvent &event)
{
    fmt::print("Mouse button {} pressed at ({}, {})\n",
               static_cast<std::size_t>(event.button),
               event.x,
               event.y);
}

void Game::on_mouse_button_released(const MouseButtonReleaseEvent &event)
{
    fmt::print("Mouse button {} released at ({}, {})\n",
               static_cast<std::size_t>(event.button),
               event.x,
               event.y);
}

void Game::on_mouse_wheel(const MouseWheelEvent &event)
{
    fmt::print("Mouse wheel scrolled by ({}, {})\n", event.x, event.y);
}

void Game::on_key_pressed(const KeyPressEvent &event)
{
    fmt::print("Key {} pressed\n", static_cast<std::size_t>(event.key_code));
}

void Game::on_key_released(const KeyReleaseEvent &event)
{
    fmt::print("Key {} released\n", static_cast<std::size_t>(event.key_code));
}

bool Game::is_mouse_button_pressed(MouseButton button) const
{
    return mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(KeyCode key_code) const
{
    return keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}
