#include "events.h"
#include "game.h"
#include "renderer.h"
#include "resources.h"
#include "sound.h"

#include <SDL2/SDL.h>
#include <fmt/format.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <unordered_map>

int main()
{
    auto frames_rendered_counter = std::size_t{ 0 };

    const auto *keyboard_state      = SDL_GetKeyboardState(nullptr);
    auto        mouse_button_states = std::array<std::uint8_t, MOUSE_BUTTON_COUNT>{};

    auto       prev_time  = std::chrono::high_resolution_clock::now();
    const auto start_time = prev_time;

    auto event = SDL_Event{};
    std::memset(&event, 0, sizeof(event));

    const auto init_res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
                                   SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    if (init_res < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    std::srand(std::time(nullptr));

    auto  game     = Game{ mouse_button_states.data(), keyboard_state };
    auto *window   = SDL_CreateWindow(PROJECT_NAME,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    640,
                                    368,
                                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    auto  renderer = Renderer{ window };
    game.load_assets(renderer);

    auto quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }

            switch (event.type)
            {
            default:
                break;

            case SDL_EventType::SDL_WINDOWEVENT: {
                switch (event.window.event)
                {
                default:
                    break;
                case SDL_WindowEventID::SDL_WINDOWEVENT_SHOWN:
                case SDL_WindowEventID::SDL_WINDOWEVENT_EXPOSED: {
                    game.on_window_shown();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_HIDDEN: {
                    game.on_window_hidden();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_MOVED: {
                    game.on_window_moved(WindowMoveEvent{ event.window.data1, event.window.data2 });
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED: {
                    game.on_window_resized(
                        WindowResizeEvent{ event.window.data1, event.window.data2 });
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_MINIMIZED:
                case SDL_WindowEventID::SDL_WINDOWEVENT_MAXIMIZED:
                case SDL_WindowEventID::SDL_WINDOWEVENT_RESTORED:
                    break;
                case SDL_WindowEventID::SDL_WINDOWEVENT_ENTER: {
                    game.on_window_mouse_entered();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_LEAVE: {
                    game.on_window_mouse_left();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_GAINED: {
                    game.on_window_focus_gained();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_LOST: {
                    game.on_window_focus_lost();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE: {
                    game.on_window_closed();
                    break;
                }
                case SDL_WindowEventID::SDL_WINDOWEVENT_TAKE_FOCUS: {
                    SDL_SetWindowInputFocus(SDL_GetWindowFromID(event.window.windowID));
                    game.on_window_focus_gained();
                    break;
                }
                }
                break;
            }

            case SDL_EventType::SDL_MOUSEMOTION: {
                game.on_mouse_moved(MouseMoveEvent{ event.motion.x, event.motion.y });
                break;
            }

            case SDL_EventType::SDL_FINGERDOWN: {
                mouse_button_states[static_cast<std::size_t>(MouseButton::Primary)] = 1;
                game.on_mouse_button_pressed(
                    MouseButtonPressEvent{ static_cast<int32_t>(event.tfinger.x),
                                           static_cast<int32_t>(event.tfinger.y),
                                           MouseButton::Primary });
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
                mouse_button_states.at(static_cast<std::size_t>(event.button.button)) = 1;
                game.on_mouse_button_pressed(
                    MouseButtonPressEvent{ event.button.x,
                                           event.button.y,
                                           static_cast<MouseButton>(event.button.button) });

                break;
            }

            case SDL_EventType::SDL_FINGERUP: {
                mouse_button_states[static_cast<std::size_t>(MouseButton::Primary)] = 0;
                game.on_mouse_button_released(
                    MouseButtonReleaseEvent{ static_cast<int32_t>(event.tfinger.x),
                                             static_cast<int32_t>(event.tfinger.y),
                                             MouseButton::Primary });
                break;
            }

            case SDL_EventType::SDL_MOUSEBUTTONUP: {
                mouse_button_states.at(static_cast<std::size_t>(event.button.button)) = 0;
                game.on_mouse_button_released(
                    MouseButtonReleaseEvent{ event.button.x,
                                             event.button.y,
                                             static_cast<MouseButton>(event.button.button) });
                break;
            }

            case SDL_EventType::SDL_MOUSEWHEEL: {
                game.on_mouse_wheel(
                    MouseWheelEvent{ event.wheel.x,
                                     event.wheel.y,
                                     event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED });
                break;
            }

            case SDL_EventType::SDL_KEYUP: {
                game.on_key_released(
                    KeyReleaseEvent{ static_cast<KeyCode>(event.key.keysym.scancode) });
                break;
            }

            case SDL_EventType::SDL_KEYDOWN: {
                game.on_key_pressed(
                    KeyPressEvent{ static_cast<KeyCode>(event.key.keysym.scancode) });
                break;
            }
            }
        }

        const auto curr_time = std::chrono::high_resolution_clock::now();
        const auto elapsed =
            static_cast<float>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time)
                    .count()) /
            1000000000.0F;
        prev_time = curr_time;

        game.render(renderer, RenderEvent{ elapsed, frames_rendered_counter++ });
        renderer.present();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
