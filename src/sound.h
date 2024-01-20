#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct Mix_Chunk;
using Mix_Music = struct _Mix_Music;

class Sound
{
private:
    struct Sample
    {
        std::int32_t                                      id;
        std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> data;
    };

    struct Music
    {
        std::int32_t                                      id;
        std::unique_ptr<Mix_Music, void (*)(Mix_Music *)> data;
    };

public:
    Sound();
    ~Sound();

    Sound(const Sound &)            = delete;
    Sound(Sound &&)                 = delete;
    Sound &operator=(const Sound &) = delete;
    Sound &operator=(Sound &&)      = delete;

    std::int32_t load_sample(const std::uint8_t *data, std::size_t size);
    std::int32_t load_music(const std::uint8_t *data, std::size_t size);

    void play_sample(std::int32_t sample_id);

    void play_music(std::int32_t music_id, std::int32_t loops = -1);
    void stop_music();

private:
    std::vector<Sample> m_loaded_samples;
    std::vector<Music>  m_loaded_music;

    std::int32_t m_current_music_id{ -1 };
};
