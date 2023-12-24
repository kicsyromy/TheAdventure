#pragma once

#include <cstdint>
#include <vector>

class Sound
{

public:
    Sound();
    ~Sound();

    Sound(const Sound &)            = delete;
    Sound(Sound &&)                 = delete;
    Sound &operator=(const Sound &) = delete;
    Sound &operator=(Sound &&)      = delete;

    void play(std::int32_t sample_id);

private:
    static void audio_callback(void *userdata, uint8_t *stream, int len);

    std::uint32_t                                       audio_device_id;
    std::vector<std::pair<std::uint8_t *, std::size_t>> audio_samples;
    std::int32_t                                        current_sample;
    std::size_t                                         current_sample_position;
};
