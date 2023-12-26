#include "sound.h"

#include <SDL2/SDL_audio.h>
#include <fmt/format.h>
#include <stb_vorbis.c>

#include <cassert>
#include <cstring>

namespace
{
    static constexpr auto AUDIO_FORMAT  = AUDIO_S16LSB;
    static constexpr auto AUDIO_SAMPLES = 4096;
} // namespace

Sound::Sound()
{}

Sound::~Sound()
{
    for (auto &sample : loaded_samples)
    {
        SDL_CloseAudioDevice(sample->audio_device_id);
    }
}

std::int32_t Sound::load_sample(const std::uint8_t *data, std::size_t size)
{
    auto data_stream = std::unique_ptr<stb_vorbis, decltype(&stb_vorbis_close)>{
        stb_vorbis_open_memory(data, static_cast<int>(size), nullptr, nullptr),
        &stb_vorbis_close
    };
    assert(data_stream != nullptr);

    const auto sample_id = static_cast<std::int32_t>(loaded_samples.size());
    auto       sample    = std::make_unique<Sample>(Sample{ sample_id, std::move(data_stream), 0 });

    auto spec = SDL_AudioSpec{};
    std::memset(&spec, 0, sizeof(SDL_AudioSpec));

    spec.freq     = sample->data_stream->sample_rate;
    spec.format   = AUDIO_FORMAT;
    spec.channels = sample->data_stream->channels;
    spec.samples  = AUDIO_SAMPLES;
    spec.callback = &Sound::audio_callback;
    spec.userdata = sample.get();

    sample->audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    assert(sample->audio_device_id != 0);

    SDL_PauseAudioDevice(sample->audio_device_id, 0);

    const auto len = stb_vorbis_stream_length_in_samples(sample->data_stream.get());
    stb_vorbis_seek(sample->data_stream.get(), len);
    loaded_samples.push_back(std::move(sample));

    return sample_id;
}

void Sound::play(std::int32_t sample_id)
{
    auto &sample = loaded_samples.at(static_cast<std::size_t>(sample_id));

    SDL_LockAudioDevice(sample->audio_device_id);

    stb_vorbis_seek_start(sample->data_stream.get());

    SDL_UnlockAudioDevice(sample->audio_device_id);
}

void Sound::audio_callback(void *userdata, uint8_t *stream, int len)
{
    auto &sample = *static_cast<Sample *>(userdata);

    std::memset(stream, 0, static_cast<std::size_t>(len));

    if (sample.data_stream->current_loc >=
        stb_vorbis_stream_length_in_samples(sample.data_stream.get()))
    {
        return;
    }

    stb_vorbis_get_samples_short_interleaved(sample.data_stream.get(),
                                             sample.data_stream->channels,
                                             reinterpret_cast<std::int16_t *>(stream),
                                             len / static_cast<std::int32_t>(sizeof(std::int16_t)));
}
