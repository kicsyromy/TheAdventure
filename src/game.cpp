#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

Game::~Game()
{}

void Game::load_assets(Renderer &renderer)
{
    image_ids.push_back(
        renderer.load_image(static_cast<const std::uint8_t *>(resource_c_plus_plus_logo),
                            resource_c_plus_plus_logo_size));
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 235, 128, 149, 255 });
    renderer.clear();

    const auto width  = renderer.width();
    const auto height = renderer.height();

    const auto logo_width  = width / 2;
    const auto logo_height = height / 2;

    renderer.draw_image(image_ids[0], logo_width / 2, logo_height / 2, logo_width, logo_height);

    fps_timer += event.seconds_per_frame;
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

bool Game::is_mouse_button_pressed(std::uint8_t button) const
{
    return mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(std::uint16_t key_code) const
{
    return keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}
