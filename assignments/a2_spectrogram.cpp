#define _USE_MATH_DEFINES

#include <fftw3.h>

#include <Eigen/Core>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "caslib/audio.hpp"
#include "caslib/colour.hpp"
#include "caslib/windows.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace cas;

struct Config {
    std::string file;
    int window;
    int hop;
    float mini;
    float maxi;
    std::string png;

    Config(int argc, char* argv[]) {
        if (argc < 6) {
            std::cerr << "Usage: ./main AUDIO.wav <N> <HOP> <cmap_min> "
                         "<cmap_max> [pngfile]"
                      << std::endl;
            exit(1);
        }

        file = argv[1];
        window = std::stoi(argv[2]);
        hop = std::stoi(argv[3]);
        mini = std::stof(argv[4]);
        maxi = std::stof(argv[5]);
        png = (argc >= 7) ? argv[6] : "output.png";
    }
};

int main(int argc, char* argv[]) {
    /***************************************************************
        Parse arguments and load audio
    ***************************************************************/
    const Config conf(argc, argv);
    const int N = conf.window;

    AudioFile<double> wav = LoadWav<double>(conf.file);
    Eigen::ArrayXd audio = wav.data.col(0);

    std::cout << "Loaded " << conf.file << std::endl;
    std::cout << "  Sample Rate: " << wav.sample_rate << std::endl;
    std::cout << "  Number of Samples: " << audio.size() << std::endl;
    std::cout << "Window size: " << conf.window << std::endl;
    std::cout << "Window hop: " << conf.hop << std::endl;

    /***************************************************************
        Pad audio to line up with window & hop
    ***************************************************************/
    // num_frames includes padding
    int num_frames = (audio.size() - N - 1) / conf.hop + 1 + 1;
    int padding = (num_frames - 1) * conf.hop + N - audio.size();

    if (padding > 0) {
        audio.conservativeResize(audio.size() + padding);
        for (auto& f : audio.tail(padding)) {
            f = 0;
        }
        std::cout << "Padded with " << padding << " zeros." << std::endl;
    } else {
        std::cout << "No padding needed." << std::endl;
    }

    Eigen::ArrayXd window = window::Blackman<double>(N);
    window /= window.sum();  // normalize to unit mass

    /***************************************************************
    Compute windowed FFT for all frames
    ***************************************************************/
    int num_bins = N / 2 + 1;
    std::cout << "Number of freq bins: " << num_bins << std::endl;
    std::cout << "Number of windows: " << num_frames << std::endl;
    Eigen::ArrayXXcd spectrogram(num_bins, num_frames);

    fftw_plan fft;

    for (int i = 0; i < num_frames; i++) {
        Eigen::ArrayXd sample = audio(Eigen::seqN(i * conf.hop, N));
        Eigen::ArrayXd windowed_sample = window * sample;

        fft = fftw_plan_dft_r2c_1d(
            N, windowed_sample.data(),
            reinterpret_cast<fftw_complex*>(spectrogram.col(i).data()),
            FFTW_ESTIMATE);

        fftw_execute(fft);
    }
    fftw_free(fft);

    Eigen::ArrayXXd spect_db = 20 * spectrogram.abs().log10();

    std::cout << "Maximum = " << spect_db.maxCoeff() << " dB" << std::endl;
    std::cout << "Minimum = " << spect_db.minCoeff() << " dB" << std::endl;

    /***************************************************************
        Create a PNG image of the spectrogram intensity
    ***************************************************************/
    std::cout << "Spectrogram is (" << num_frames << " x " << num_bins
              << ") pixels." << std::endl;

    auto palette = colour::sunset;
    palette.Rescale(conf.mini, conf.maxi);

    uint8_t* image_buffer = new uint8_t[num_frames * num_bins * 3];
    for (int sp = 0; sp < num_frames; sp++) {
        for (int i = 0; i < num_bins; i++) {
            int idx = ((num_bins - 1 - i) * num_frames + sp) * 3;

            double v = spect_db(i, sp);

            auto col = palette.Get(v);

            image_buffer[idx] = col.red;
            image_buffer[idx + 1] = col.green;
            image_buffer[idx + 2] = col.blue;
        }
    }

    stbi_write_png(conf.png.c_str(), num_frames, num_bins, 3, image_buffer,
                   num_frames * 3);
    std::cout << "Saved image to " << conf.png << std::endl;

    return 0;
}