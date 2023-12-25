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

        std::unique_ptr<stb_vorbis, void (*)(stb_vorbis *)> data_stream;
        std::int32_t                                        position;

        std::uint32_t audio_device_id;
        Sound        &self;
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

    std::vector<std::unique_ptr<Sample>> loaded_samples;
};
