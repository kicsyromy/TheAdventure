#pragma once

#include "codes.h"
#include "sound.h"

#include <cstdint>
#include <random>
#include <vector>

class Renderer;
struct RenderEvent;
struct WindowMoveEvent;
struct WindowResizeEvent;
struct MouseMoveEvent;
struct MouseButtonPressEvent;
struct MouseButtonReleaseEvent;
struct MouseWheelEvent;
struct KeyPressEvent;
struct KeyReleaseEvent;

class Game final
{
public:
    template<typename MBS, typename KS>
    Game(const MBS &mouse_button_states, const KS &keyboard_state)
      : mouse_button_states(mouse_button_states)
      , keyboard_state(keyboard_state)
    {}

    ~Game();

    Game(const Game &other)            = delete;
    Game(Game &&other)                 = delete;
    Game &operator=(const Game &other) = delete;
    Game &operator=(Game &&other)      = delete;

    void load_assets(Renderer &renderer);

    void render(Renderer &renderer, const RenderEvent &event);

    void on_window_shown();
    void on_window_hidden();
    void on_window_moved(const WindowMoveEvent &event);
    void on_window_resized(const WindowResizeEvent &event);
    void on_window_focus_gained();
    void on_window_focus_lost();
    void on_window_closed();
    void on_window_mouse_entered();
    void on_window_mouse_left();
    void on_mouse_moved(const MouseMoveEvent &event);
    void on_mouse_button_pressed(const MouseButtonPressEvent &event);
    void on_mouse_button_released(const MouseButtonReleaseEvent &event);
    void on_mouse_wheel(const MouseWheelEvent &event);
    void on_key_pressed(const KeyPressEvent &event);
    void on_key_released(const KeyReleaseEvent &event);

private:
    bool is_mouse_button_pressed(MouseButton button) const;
    bool is_key_pressed(KeyCode key_code) const;

    const std::uint8_t *const mouse_button_states;
    const std::uint8_t *const keyboard_state;

    Sound sound;

    float       total_seconds_elapsed{ 0.F };
    float       fps_timer{ 0.F };
    std::size_t frame_counter{ 0 };

    std::int32_t       res_cpp_logo;
    const std::int32_t logo_width{ 200 };
    const std::int32_t logo_height{ 200 };
    float              logo_x{ 200.F };
    float              sign_x{ 1.F };
    float              logo_y{ 200.F };
    float              sign_y{ 1.F };
    const float        delta_x{ static_cast<float>(rand() % 200) + 70.F };
    const float        delta_y{ static_cast<float>(rand() % 200) + 70.F };

    std::int32_t res_ding;
};
