#pragma once

#include <Eigen/Core>
#include <filesystem>

#include "sndfile.hh"

namespace cas {

template <typename T>
struct AudioFile {
    int sample_rate;
    Eigen::ArrayXX<T> data;
};

template <typename T>
AudioFile<T> LoadWav(std::string filename) {
    SndfileHandle f(filename);

    if (f.error()) {
        throw std::filesystem::filesystem_error(
            f.strError(), std::make_error_code(std::errc::io_error));
    }

    size_t num_chn = f.channels();
    size_t num_frames = f.frames();

    // Create AudioFile
    AudioFile<T> audio;
    audio.sample_rate = f.samplerate();

    // read audio into buffer
    T* buffer = new T[num_chn * num_frames]();
    f.readf(buffer, num_frames);

    audio.data = Eigen::Map<Eigen::ArrayXX<T>>(buffer, num_chn, num_frames);
    audio.data.transposeInPlace();

    return audio;
}

}  // namespace cas