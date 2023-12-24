#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

Game::~Game()
{}

void Game::load_assets(Renderer &renderer)
{
    res_cpp_logo = renderer.load_image(static_cast<const std::uint8_t *>(resource_c_plus_plus_logo),
                                       resource_c_plus_plus_logo_size);

    res_ding =
        sound.load_sample(static_cast<const std::uint8_t *>(resource_ding), resource_ding_size);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    const auto red =
        static_cast<std::uint8_t>(255.F * ((std::sin(total_seconds_elapsed) + 1.F) / 2.F));
    const auto green =
        static_cast<std::uint8_t>(255.F * ((std::cos(total_seconds_elapsed) + 1.F) / 2.F));

    renderer.set_color({ red, green, 149, 255 });
    renderer.clear();

    if (!is_key_pressed(KeyCode::Space))
    {
        logo_x += sign_x * delta_x * event.seconds_elapsed;
        logo_y += sign_y * delta_y * event.seconds_elapsed;

        if (logo_x >= renderer.width() - logo_width)
        {
            logo_x = renderer.width() - logo_width;
            sign_x = -1.F;

            sound.play(res_ding);
        }
        else if (logo_x <= 0)
        {
            logo_x = 0;
            sign_x = 1.F;

            sound.play(res_ding);
        }

        if (logo_y >= renderer.height() - logo_height)
        {
            logo_y = renderer.height() - logo_height;
            sign_y = -1.F;

            sound.play(res_ding);
        }
        else if (logo_y <= 0)
        {
            logo_y = 0;
            sign_y = 1.F;

            sound.play(res_ding);
        }
    }

    renderer.draw_image(res_cpp_logo, logo_x, logo_y, logo_width, logo_height);

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
