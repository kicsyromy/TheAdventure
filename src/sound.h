#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct Mix_Chunk;

class Sound
{
private:
    struct Sample
    {
        std::int32_t                                      id;
        std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> data;
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
    std::vector<Sample> loaded_samples;
};
