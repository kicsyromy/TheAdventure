#include "game.h"
#include "events.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

#include <algorithm>
#include <unordered_set>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_map.emplace(renderer);

    m_bg_music_id = m_sound.load_music(resource_world_bg, resource_world_bg_size);
    m_sound.play_music(m_bg_music_id);

    for (int i = 0; i < 5; ++i)
    {
        auto *slime = new Slime{ renderer, m_sound };
        slime->x() += i * 64;
        slime->y() += i * 45;
        m_things.emplace_back(slime->id(), slime);
        m_renderables.emplace_back(slime->id(), slime);
        m_collidables.emplace_back(slime->id(), slime);
        m_destroyables.emplace_back(slime->id(), slime);
    }

    auto *hero = new Hero{ renderer, m_sound };
    m_things.emplace_back(hero->id(), hero);
    m_renderables.emplace_back(hero->id(), hero);
    m_collidables.emplace_back(hero->id(), hero);
    m_input_handlers.emplace_back(hero->id(), hero);
    m_attackers.emplace_back(hero->id(), hero);
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 0, 0, 0, 255 });
    renderer.clear();

    m_map->render();

    std::vector<std::size_t> destroyed_indices;
    for (std::size_t i = 0; i < m_destroyables.size(); ++i)
    {
        if (m_destroyables[i].second->should_be_destroyed())
        {
            const auto id = m_destroyables[i].first;

            destroyed_indices.push_back(i);

            auto render_it =
                std::find_if(m_renderables.begin(), m_renderables.end(), [id](auto &renderable) {
                    return renderable.first == id;
                });
            if (render_it != m_renderables.end())
            {
                m_renderables.erase(render_it);
            }

            auto collide_it =
                std::find_if(m_collidables.begin(), m_collidables.end(), [id](auto &collidable) {
                    return collidable.first == id;
                });
            if (collide_it != m_collidables.end())
            {
                m_collidables.erase(collide_it);
            }

            auto input_it =
                std::find_if(m_input_handlers.begin(),
                             m_input_handlers.end(),
                             [id](auto &input_handler) { return input_handler.first == id; });
            if (input_it != m_input_handlers.end())
            {
                m_input_handlers.erase(input_it);
            }

            auto attack_it = std::find_if(m_attackers.begin(),
                                          m_attackers.end(),
                                          [id](auto &attacker) { return attacker.first == id; });
            if (attack_it != m_attackers.end())
            {
                m_attackers.erase(attack_it);
            }

            auto thing_it = std::find_if(m_things.begin(), m_things.end(), [id](auto &thing) {
                return thing.first == id;
            });
            m_things.erase(thing_it);
        }
    }

    for (auto i = 0; i < destroyed_indices.size(); ++i)
    {
        m_destroyables.erase(m_destroyables.begin() + destroyed_indices[i] - i);
    }

    for (const auto &[id, thing] : m_things)
    {
        thing->update(*this, event.seconds_elapsed);
    }

    for (const auto &[id, renderable] : m_renderables)
    {
        renderable->render(renderer);
    }

    std::unordered_set<ICollidable *> colliding;
    std::unordered_set<ICollidable *> not_colliding;
    not_colliding.reserve(m_collidables.size());
    for (std::size_t i = 0; i < m_collidables.size(); ++i)
    {
        not_colliding.insert(m_collidables[i].second);
    }

    for (std::size_t i = 0; i < m_collidables.size() - 1; ++i)
    {
        for (std::size_t j = i + 1; j < m_collidables.size(); ++j)
        {
            if (m_collidables[i].second->is_colliding(*m_collidables[j].second))
            {
                const auto i_id = m_collidables[i].first;
                const auto j_id = m_collidables[j].first;

                auto attacker_i_it =
                    std::find_if(m_attackers.begin(), m_attackers.end(), [i_id](auto &attacker) {
                        return attacker.first == i_id;
                    });
                if (attacker_i_it != m_attackers.end() && attacker_i_it->second->is_attacking())
                {
                    auto dest_it = std::find_if(m_destroyables.begin(),
                                                m_destroyables.end(),
                                                [j_id](auto &dest) { return dest.first == j_id; });
                    if (dest_it != m_destroyables.end())
                    {
                        dest_it->second->take_damage(attacker_i_it->second->attack_power() *
                                                     event.seconds_elapsed);
                    }
                }

                auto attacker_j_jt =
                    std::find_if(m_attackers.begin(), m_attackers.end(), [j_id](auto &attacker) {
                        return attacker.first == j_id;
                    });
                if (attacker_j_jt != m_attackers.end() && attacker_j_jt->second->is_attacking())
                {
                    auto dest_it = std::find_if(m_destroyables.begin(),
                                                m_destroyables.end(),
                                                [i_id](auto &dest) { return dest.first == i_id; });
                    if (dest_it != m_destroyables.end())
                    {
                        dest_it->second->take_damage(attacker_j_jt->second->attack_power() *
                                                     event.seconds_elapsed);
                    }
                }

                colliding.insert(m_collidables[i].second);
                colliding.insert(m_collidables[j].second);

                not_colliding.erase(m_collidables[i].second);
                not_colliding.erase(m_collidables[j].second);
            }
        }
    }

    // for (const auto &collidable : colliding)
    // {
    //     collidable->render_collision_box(renderer, true);
    // }

    // for (const auto &collidable : not_colliding)
    // {
    //     collidable->render_collision_box(renderer, false);
    // }

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
{
    event.renderer.update_backbuffer_size();
}

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
    for (auto &[id, input_handler] : m_input_handlers)
    {
        input_handler->on_key_pressed(event);
    }
}

void Game::on_key_released(const KeyReleaseEvent &event)
{
    for (auto &[id, input_handler] : m_input_handlers)
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
