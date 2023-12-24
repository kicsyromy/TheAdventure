#include "sound.h"

#include <SDL2/SDL_audio.h>
#include <fmt/format.h>
#include <stb_vorbis.c>

#include <cassert>
#include <cstring>

namespace
{
    static constexpr auto AUDIO_FORMAT    = AUDIO_S16LSB;
    static constexpr auto AUDIO_FREQUENCY = 44100;
    static constexpr auto AUDIO_CHANNELS  = 2;
    static constexpr auto AUDIO_SAMPLES   = 4096;
} // namespace

Sound::Sound()
{
    auto spec = SDL_AudioSpec{};
    std::memset(&spec, 0, sizeof(SDL_AudioSpec));

    spec.freq     = AUDIO_FREQUENCY;
    spec.format   = AUDIO_FORMAT;
    spec.channels = AUDIO_CHANNELS;
    spec.samples  = AUDIO_SAMPLES;
    spec.callback = &Sound::audio_callback;
    spec.userdata = this;

    audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    assert(audio_device_id != 0);

    SDL_PauseAudioDevice(audio_device_id, 0);
}

Sound::~Sound()
{
    SDL_CloseAudioDevice(audio_device_id);
}

std::int32_t Sound::load_sample(const std::uint8_t *data, std::size_t size)
{
    auto sample = std::unique_ptr<stb_vorbis, decltype(&stb_vorbis_close)>{
        stb_vorbis_open_memory(data, static_cast<int>(size), nullptr, nullptr),
        &stb_vorbis_close
    };
    assert(sample != nullptr);

    const auto sample_id = static_cast<std::int32_t>(loaded_samples.size());
    loaded_samples.push_back(std::move(sample));

    return sample_id;
}

void Sound::play(std::int32_t sample_id)
{
    SDL_LockAudioDevice(audio_device_id);

    playing_samples.push_back({ sample_id, 0 });

    SDL_UnlockAudioDevice(audio_device_id);
}

void Sound::audio_callback(void *userdata, uint8_t *stream, int len)
{
    auto &self = *static_cast<Sound *>(userdata);

    std::memset(stream, 0, static_cast<std::size_t>(len));

    if (self.playing_samples.empty())
    {
        return;
    }

    if (self.current_sample >= self.playing_samples.size())
    {
        self.current_sample = 0;
    }

    auto &current_sample = self.playing_samples.at(self.current_sample);
    auto &current_stream = self.loaded_samples.at(static_cast<std::size_t>(current_sample.id));

    stb_vorbis_seek_start(current_stream.get());
    stb_vorbis_seek(current_stream.get(), current_sample.position);
    const auto res = stb_vorbis_get_samples_short_interleaved(
        current_stream.get(),
        AUDIO_CHANNELS,
        reinterpret_cast<std::int16_t *>(stream),
        len / static_cast<std::int32_t>(sizeof(std::int16_t)));
    if (res == 0)
    {
        self.playing_samples.erase(self.playing_samples.begin() + self.current_sample);
    }
    else
    {
        current_sample.position += res;
        if (current_sample.position >= stb_vorbis_stream_length_in_samples(current_stream.get()))
        {
            self.playing_samples.erase(self.playing_samples.begin() + self.current_sample);
        }
        else
        {
            ++self.current_sample;
        }
    }
}
