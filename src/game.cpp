#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

#include <unordered_set>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_bg_music_id = m_sound.load_music(resource_world_bg, resource_world_bg_size);
    m_sound.play_music(m_bg_music_id);

    for (int i = 0; i < 5; ++i)
    {
        auto *slime = new Slime{ renderer, m_sound };
        slime->x() += i * 64;
        slime->y() += i * 45;
        m_things.emplace_back(slime);
        m_renderables.push_back(slime);
        m_collidables.push_back(slime);
    }

    auto *hero = new Hero{ renderer, m_sound };
    m_things.emplace_back(hero);
    m_renderables.push_back(hero);
    m_collidables.push_back(hero);
    m_input_handlers.push_back(hero);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 127, 127, 127, 255 });
    renderer.clear();

    for (const auto &thing : m_things)
    {
        thing->update(*this, event.seconds_elapsed);
    }

    for (const auto &renderable : m_renderables)
    {
        renderable->render(renderer);
    }

    std::unordered_set<ICollidable *> colliding;
    std::unordered_set<ICollidable *> not_colliding{ m_collidables.begin(), m_collidables.end() };
    for (std::size_t i = 0; i < m_collidables.size() - 1; ++i)
    {
        for (std::size_t j = i + 1; j < m_collidables.size(); ++j)
        {
            if (m_collidables[i]->is_colliding(*m_collidables[j]))
            {
                colliding.insert(m_collidables[i]);
                colliding.insert(m_collidables[j]);

                not_colliding.erase(m_collidables[i]);
                not_colliding.erase(m_collidables[j]);
            }
        }
    }

    for (const auto &collidable : colliding)
    {
        collidable->render_collision_box(renderer, true);
    }

    for (const auto &collidable : not_colliding)
    {
        collidable->render_collision_box(renderer, false);
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
    for (auto &input_handler : m_input_handlers)
    {
        input_handler->on_key_pressed(event);
    }
}

void Game::on_key_released(const KeyReleaseEvent &event)
{
    for (auto &input_handler : m_input_handlers)
    {
        input_handler->on_key_released(event);
    }
}

bool Game::is_mouse_button_pressed(MouseButton button) const
{
    return m_mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(KeyCode key_code) const
{
    return m_keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}
