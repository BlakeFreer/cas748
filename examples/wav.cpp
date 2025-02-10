#include "caslib/file/wav.hpp"

#include <iostream>
#include <vector>

const size_t BUFFER_LEN = 1024;
int main() {
    cas::AudioFile wav = cas::ReadWav("in5.wav");

    // Print all samples in the only channel
    for (auto f : wav.channels[0]) {
        std::cout << f << std::endl;
    }

    std::cout << "Sample rate: " << wav.sample_rate << std::endl;
    std::cout << "Num Channels: " << wav.channels.size() << std::endl;
    std::cout << "Num Frames: " << wav.channels[0].size() << std::endl;

    return 0;
}