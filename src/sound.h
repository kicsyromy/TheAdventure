#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct stb_vorbis;

class Sound
{
private:
    struct Sample
    {
        std::int32_t id;
        std::int32_t position;
    };

public:
    Sound();
    ~Sound();

    Sound(const Sound &)            = delete;
    Sound(Sound &&)                 = delete;
    Sound &operator=(const Sound &) = delete;
    Sound &operator=(Sound &&)      = delete;

    std::int32_t load_sample(const std::uint8_t *data, std::size_t size);
    void         play(std::int32_t sample_id);

private:
    static void audio_callback(void *userdata, uint8_t *stream, int len);

    std::uint32_t                                                    audio_device_id;
    std::vector<std::unique_ptr<stb_vorbis, void (*)(stb_vorbis *)>> loaded_samples;
    std::vector<Sample>                                              playing_samples;
    std::size_t                                                      current_sample;
};
