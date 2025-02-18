
#include <algorithm>
#include <iostream>
#include <vector>

#include "caslib/analyze.hpp"
#include "caslib/colour.hpp"
#include "caslib/file/wav.hpp"
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

    Config(int argc, char* argv[]) {
        if (argc != 6) {
            std::cerr
                << "Usage: ./main AUDIO.wav <N> <HOP> <cmap_min> <cmap_max>"
                << std::endl;
            exit(1);
        }

        file = argv[1];
        window = std::stoi(argv[2]);
        hop = std::stoi(argv[3]);
        mini = std::stof(argv[4]);
        maxi = std::stof(argv[5]);
    }
};

int main(int argc, char* argv[]) {
    const Config conf(argc, argv);
    const int N = conf.window;

    AudioFile wav = ReadWav(conf.file);
    Series audio = wav.channels[0];

    // audio = GenerateSamples(Sine(1, 256), 16000, 4096);
    // audio = GenerateSamples(WhiteNoise(1), 16000, 8000);

    std::cout << "Loaded " << conf.file << std::endl;
    std::cout << "  Sample Rate: " << wav.sample_rate << std::endl;
    std::cout << "  Number of Samples: " << audio.size() << std::endl;
    std::cout << "Window size: " << conf.window << std::endl;
    std::cout << "Window hop: " << conf.hop << std::endl;

    Series window = window::Blackman(N);

    float mini = std::numeric_limits<float>::max();
    float maxi = std::numeric_limits<float>::lowest();

    // Compute the FT spectrum over time
    std::vector<Series> spectrum;
    for (int start = 0; start + N <= audio.size(); start += conf.hop) {
        Series sample = audio.View(start, start + N);
        Series ft = FourierTransform(window * sample).Apply([](auto f) {
            return 10 * std::log10(f);
        });
        spectrum.push_back(ft);

        mini = std::min(mini, *std::min_element(ft.begin(), ft.end()));
        maxi = std::max(maxi, *std::max_element(ft.begin(), ft.end()));
    }

    std::cout << "Maximum = " << maxi << " dB" << std::endl;
    std::cout << "Minimum = " << mini << " dB" << std::endl;

    // Create image buffer
    int width = spectrum.size();
    int height = spectrum[0].size();
    std::cout << "Spectrum PNG is (" << width << " x " << height << ")"
              << std::endl;

    std::vector<unsigned char> image(width * height * 3);  // RGB image buffer

    auto palette = colour::sunset;
    palette.Rescale(conf.mini, conf.maxi);

    for (int sp = 0; sp < width; sp++) {
        for (int i = 0; i < height; i++) {
            int idx = ((height - 1 - i) * width + sp) * 3;

            double v = spectrum[sp][i];

            auto col = palette.Get(v);
            image[idx] = col.red;
            image[idx + 1] = col.green;
            image[idx + 2] = col.blue;
        }
    }

    stbi_write_png("output.png", width, height, 3, image.data(), width * 3);

    return 0;
}