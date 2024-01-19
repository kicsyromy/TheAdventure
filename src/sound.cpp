#include "sound.h"

#include <SDL2/SDL_audio.h>
#include <SDL_mixer.h>
#include <fmt/format.h>
// #include <stb_vorbis.c>

#include <cassert>
#include <cstring>

namespace
{
    static constexpr auto AUDIO_FORMAT  = AUDIO_S16SYS;
    static constexpr auto AUDIO_SAMPLES = 4096;
} // namespace

Sound::Sound()
{
    Mix_OpenAudio(44100, AUDIO_FORMAT, 2, 2048);
    Mix_AllocateChannels(8);
}

Sound::~Sound()
{
    Mix_CloseAudio();
}

std::int32_t Sound::load_sample(const std::uint8_t *data, std::size_t size)
{
    auto sample = Sample{ -1, { nullptr, nullptr } };

    sample.data = std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)>{
        Mix_LoadWAV_RW(SDL_RWFromConstMem(data, size), 1),
        &Mix_FreeChunk
    };

    if (!sample.data)
    {
        fmt::print(stderr, "Unable to load sample: {}\n", Mix_GetError());
        return -1;
    }

    const auto sample_id = static_cast<std::int32_t>(loaded_samples.size());
    sample.id            = sample_id;
    loaded_samples.push_back(std::move(sample));

    return sample_id;
}

void Sound::play(std::int32_t sample_id)
{
    assert(sample_id >= 0 && sample_id < static_cast<std::int32_t>(loaded_samples.size()));

    Mix_PlayChannel(-1, loaded_samples[sample_id].data.get(), 0);
}
