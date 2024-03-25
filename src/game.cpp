#include "game.h"
#include "events.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_destroyable.h"
#include "i_input_handler.h"
#include "i_renderable.h"
#include "i_thing.h"
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
    //    m_sound.play_music(m_bg_music_id);

    auto *hero       = new Hero{ renderer, m_sound };
    hero->x()        = 500;
    hero->render_x() = hero->x();
    m_things.emplace(hero->id(), hero);

    for (int i = 0; i < 1; ++i)
    {
        auto *slime = new Slime{ renderer, m_sound };
        slime->x() += i * 64;
        slime->render_x() = slime->x();
        slime->y() += i * 45;
        slime->render_y() = slime->y();

        slime->set_aggravated_by(*hero);

        m_things.emplace(slime->id(), slime);
    }

    m_hero = hero;
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 0, 0, 0, 255 });
    renderer.clear();

    const auto [offset_x, offset_y] = m_map->update(m_hero->render_x(),
                                                    m_hero->render_y(),
                                                    m_hero->speed(),
                                                    event.seconds_elapsed);
    m_map->render();

    std::vector<std::size_t> destroyed_ids;
    for (const auto &[id, thing] : m_things)
    {
        if (auto *destroyable = dynamic_cast<IDestroyable *>(thing.get()))
        {
            if (destroyable->should_be_destroyed())
            {
                destroyed_ids.push_back(id);
            }
        }
    }

    for (auto i = 0; i < destroyed_ids.size(); ++i)
    {
        m_things.erase(destroyed_ids[i]);
    }

    for (const auto &[id, thing] : m_things)
    {
        thing->update(*this, event.seconds_elapsed);

        thing->x() = std::clamp(thing->x(), 0.F, m_map->width() - thing->width() / 2.F);
        thing->y() = std::clamp(thing->y(), 0.F, m_map->height() - thing->height() / 2.F);

        if (auto *renderable = dynamic_cast<IRenderable *>(thing.get()))
        {
            renderable->render_x() = renderable->render_x() + offset_x;
            renderable->render_y() = renderable->render_y() + offset_y;

            if (id == m_hero->id())
            {
                m_hero->render_x() =
                    std::clamp(m_hero->render_x(), 0.F, static_cast<float>(renderer.width()));
                m_hero->render_y() =
                    std::clamp(m_hero->render_y(), 0.F, static_cast<float>(renderer.height()));
            }
            else
            {
                renderable->render(renderer);
            }
        }
    }

    m_hero->render(renderer);

    std::unordered_set<ICollidable *> colliding;
    colliding.reserve(m_things.size());
    std::unordered_set<ICollidable *> not_colliding;
    not_colliding.reserve(m_things.size());
    std::vector<std::pair<std::int32_t, ICollidable *>> collidables;
    collidables.reserve(m_things.size());
    for (const auto &[id, thing] : m_things)
    {
        if (auto *collidable = dynamic_cast<ICollidable *>(thing.get()))
        {
            not_colliding.insert(collidable);
            collidables.emplace_back(id, collidable);
        }
    }
    for (std::size_t i = 0; i < collidables.size() - 1; ++i)
    {
        for (std::size_t j = i + 1; j < collidables.size(); ++j)
        {
            if (collidables[i].second->is_colliding(*collidables[j].second))
            {
                const auto i_id = collidables[i].first;
                const auto j_id = collidables[j].first;

                try_attack(i_id, j_id);
                try_attack(j_id, i_id);

                colliding.insert(collidables[i].second);
                colliding.insert(collidables[j].second);

                not_colliding.erase(collidables[i].second);
                not_colliding.erase(collidables[j].second);
            }
        }
    }

    //    for (const auto &collidable : colliding)
    //    {
    //        collidable->render_collision_box(renderer, true);
    //    }
    //
    //    for (const auto &collidable : not_colliding)
    //    {
    //        collidable->render_collision_box(renderer, false);
    //    }

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
    for (auto &[id, thing] : m_things)
    {
        if (auto *input_handler = dynamic_cast<IInputHandler *>(thing.get()))
        {
            input_handler->on_key_pressed(event);
        }
    }
}

void Game::on_key_released(const KeyReleaseEvent &event)
{
    for (auto &[id, thing] : m_things)
    {
        if (auto *input_handler = dynamic_cast<IInputHandler *>(thing.get()))
        {
            input_handler->on_key_released(event);
        }
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

void Game::try_attack(std::int32_t thing_id_1, std::int32_t thing_id_2)
{
    // Find out if thing_1 is an attacker
    const auto attacker_it = m_things.find(thing_id_1);
    if (attacker_it != m_things.end()) // thing_1 is an attacker
    {
        const auto *attacker = dynamic_cast<IAttacker *>(attacker_it->second.get());
        if (attacker != nullptr && attacker->is_attacking()) // thing_1 is attacking
        {
            // We are in an attack phase

            // Find out if our target is a destroyable
            auto dest_it = m_things.find(thing_id_2);
            if (dest_it != m_things.end()) // Our target is a destroyable

            {
                auto *dest = dynamic_cast<IDestroyable *>(dest_it->second.get());
                if (dest == nullptr)
                {
                    return;
                }

                // Find out if the attacker has already landed an attack on some target
                auto attack_landed = m_landed_attacks.find(attacker_it->first);
                if (attack_landed != m_landed_attacks.end()) // The attacker has already
                                                             // landed an attack on some
                                                             // target
                {
                    // Find out if the attacker has already landed an attack on our
                    // target
                    const auto target_hit = attack_landed->second.find(dest_it->first);
                    if (target_hit == attack_landed->second.end()) // The attacker has not
                                                                   // landed an attack on our
                                                                   // target
                    {
                        // Mark the target as hit in this attack phase
                        attack_landed->second.insert(dest_it->first);
                        // Deal damage to the target
                        dest->take_damage(attacker->attack_power());
                    }
                }
                else // The attacker has just started their attack phase
                {
                    // Mark the attacker as having landed an attack on our target
                    // Add the attacker to the landed attacks and mark the target as hit
                    m_landed_attacks.emplace(attacker_it->first,
                                             std::unordered_set<int32_t>{ dest_it->first });
                    // Deal damage to the target
                    dest->take_damage(attacker->attack_power());
                }
            }
        }
        else // thing_1 is not attacking
        {
            // thing_1's attack period has ended
            // Remove the attacker from the landed attacks
            m_landed_attacks.erase(attacker_it->first);
        }
    }
}
