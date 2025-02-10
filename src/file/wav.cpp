#include "caslib/file/wav.hpp"

#include <cassert>
#include <filesystem>

#include "caslib/file/audio.hpp"
#include "sndfile.hh"

namespace cas {

AudioFile ReadWav(std::string filename) {
    // this would be a great use of iterator views. hold all of the samples in a
    // single Series and let each channel view into the data at different
    // offsets
    SndfileHandle f(filename);

    if (f.error()) {
        throw std::filesystem::filesystem_error(
            f.strError(), std::make_error_code(std::errc::io_error));
    }

    size_t num_chn = f.channels();
    size_t num_frames = f.frames();

    // read audio into buffer
    float* buffer = new float[num_chn * num_frames]();
    f.readf(buffer, num_frames);

    // Create a Series for each channels
    std::vector<Series> channels;
    for (size_t i = 0; i < num_chn; i++) {
        Series s{
            std::make_shared<float[]>(num_frames),
            static_cast<size_t>(num_frames),
        };

        // extract the interlaced data
        for (size_t frame = 0; frame < num_frames; frame++) {
            s[frame] = buffer[frame * num_chn + i];
        }

        channels.push_back(s);
    }

    return AudioFile{
        .sample_rate = f.samplerate(),
        .channels = channels,
    };
}

}  // namespace cas