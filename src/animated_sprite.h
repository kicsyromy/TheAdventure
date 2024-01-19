#pragma once

#include "sprite.h"

#include <chrono>
#include <cstdint>

class AnimatedSprite final : public Sprite
{
public:
    using Sprite::Sprite;

    void render(Renderer &renderer) override;

    void set_sprite_set(std::int32_t sprite_set, bool flip = false);
    void set_total_frames(std::int32_t total_frames, std::int32_t exclude_frames = 0);
    void set_frame_time(std::chrono::milliseconds frame_time);
    void reset();

private:
    std::int32_t m_sprite_set{ 0 };
    std::int32_t m_total_frames{ 0 };
    std::int32_t m_frame_size{ 0 };
    bool         m_flip{ false };

    std::int32_t                                       m_current_frame{ 0 };
    std::chrono::milliseconds                          m_frame_time{ 0 };
    std::chrono::time_point<std::chrono::steady_clock> m_last_frame_time{
        std::chrono::steady_clock::now()
    };
};