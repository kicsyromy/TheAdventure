#include "sound.h"

#include <SDL2/SDL_audio.h>
#include <SDL_mixer.h>
#include <fmt/format.h>
// #include <stb_vorbis.c>

#include <cassert>
#include <cstring>

namespace
{
    Sound *SOUND_INSTANCE = nullptr;
} // namespace

Sound::Sound()
{
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    Mix_AllocateChannels(8);
    Mix_VolumeMusic(100);

    SOUND_INSTANCE = this;
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

    const auto sample_id = static_cast<std::int32_t>(m_loaded_samples.size());
    sample.id            = sample_id;
    m_loaded_samples.push_back(std::move(sample));

    return sample_id;
}

std::int32_t Sound::load_music(const std::uint8_t *data, std::size_t size)
{
    auto music = Music{ -1, { nullptr, nullptr } };

    music.data = std::unique_ptr<Mix_Music, void (*)(Mix_Music *)>{
        Mix_LoadMUS_RW(SDL_RWFromConstMem(data, size), 1),
        &Mix_FreeMusic
    };

    if (!music.data)
    {
        fmt::print(stderr, "Unable to load music: {}\n", Mix_GetError());
        return -1;
    }

    const auto music_id = static_cast<std::int32_t>(m_loaded_music.size());
    music.id            = music_id;
    m_loaded_music.push_back(std::move(music));

    return music_id;
}

void Sound::play_sample(std::int32_t sample_id)
{
    assert(sample_id >= 0 && sample_id < static_cast<std::int32_t>(m_loaded_samples.size()));

    Mix_PlayChannel(-1, m_loaded_samples[sample_id].data.get(), 0);
}

void Sound::play_music(std::int32_t music_id, std::int32_t loops)
{
    assert(music_id >= 0 && music_id < static_cast<std::int32_t>(m_loaded_music.size()));

    m_current_music_id = music_id;

    if (loops < 0)
    {
        Mix_HookMusicFinished([]() {
            auto &self = *SOUND_INSTANCE;
            Mix_PlayMusic(self.m_loaded_music[self.m_current_music_id].data.get(), 0);
        });

        loops = 0;
    }
    else
    {
        Mix_HookMusicFinished(nullptr);
    }

    Mix_PlayMusic(m_loaded_music[music_id].data.get(), loops);
}

void Sound::stop_music()
{
    Mix_HookMusicFinished(nullptr);
    m_current_music_id = -1;

    Mix_HaltMusic();
}