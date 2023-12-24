#include "sound.h"

#include <SDL2/SDL_audio.h>
#include <fmt/format.h>

#include <cassert>
#include <cstring>

namespace
{
    static constexpr auto AUDIO_FORMAT           = AUDIO_S16LSB;
    static constexpr auto AUDIO_FREQUENCY        = 48000;
    static constexpr auto AUDIO_CHANNELS         = 2;
    static constexpr auto AUDIO_SAMPLES          = 4096;
    static constexpr auto AUDIO_MAX_SOUNDS       = 25;
    static constexpr auto AUDIO_MUSIC_FADE_VALUE = 2;
} // namespace

Sound::Sound()
{
    auto spec = SDL_AudioSpec{};
    std::memset(&spec, 0, sizeof(SDL_AudioSpec));

    //    spec.freq     = AUDIO_FREQUENCY;
    //    spec.format   = AUDIO_FORMAT;
    //    spec.channels = AUDIO_CHANNELS;
    //    spec.samples  = AUDIO_SAMPLES;
    //    spec.callback = &Sound::audio_callback;
    //    spec.userdata = this;

    std::uint8_t *audio_buffer = nullptr;
    std::uint32_t buffer_size  = 0;

    const auto *res = SDL_LoadWAV("C:\\Users\\kicsyromy\\Documents\\TheAdventure\\res\\ding.wav",
                                  &spec,
                                  &audio_buffer,
                                  &buffer_size);
    assert(res != nullptr);

    spec.callback   = &Sound::audio_callback;
    spec.userdata   = this;
    audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    assert(audio_device_id != 0);

    audio_samples.emplace_back(audio_buffer, static_cast<std::size_t>(buffer_size));
}

Sound::~Sound()
{
    SDL_CloseAudioDevice(audio_device_id);
}

void Sound::play(std::int32_t sample_id)
{
    SDL_LockAudioDevice(audio_device_id);

    current_sample          = sample_id;
    current_sample_position = 0;

    SDL_UnlockAudioDevice(audio_device_id);

    SDL_PauseAudioDevice(audio_device_id, 0);
}

void Sound::audio_callback(void *userdata, uint8_t *stream, int len)
{
    auto &self = *static_cast<Sound *>(userdata);

    std::memset(stream, 0, static_cast<std::size_t>(len));

    const auto &current_sample = self.audio_samples.at(self.current_sample);

    if (self.current_sample_position >= current_sample.second)
    {
        fmt::print("Done!\n");
        SDL_PauseAudioDevice(self.audio_device_id, 1);
        return;
    }

    fmt::print("sample_pos: {}\n", self.current_sample_position);

    len = std::min(static_cast<std::int32_t>(current_sample.second - self.current_sample_position),
                   len);
    fmt::print("len : {}\n", len);
    SDL_MixAudioFormat(stream,
                       current_sample.first + self.current_sample_position,
                       AUDIO_FORMAT,
                       len,
                       SDL_MIX_MAXVOLUME);
    self.current_sample_position += len;
}
