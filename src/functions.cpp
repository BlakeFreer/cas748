#define _USE_MATH_DEFINES
#include <math.h>

#include <random>

#include "caslib/functions.hpp"
#include "caslib/series.hpp"

namespace cas {

Function Sine(const float amplitude, const float frequency) {
    return [=](const float time) {
        return amplitude * sin(2 * M_PI * frequency * time);
    };
}

Function WhiteNoise(const float amplitude) {
    static std::random_device seed;
    static std::mt19937 generator(seed());
    static std::uniform_real_distribution<float> dist(-1, 1);
    return [=](const float time) { return amplitude * dist(generator); };
}

Function GaussianNoise(const float sigma) {
    static std::random_device seed;
    static std::mt19937 generator(seed());
    static std::normal_distribution<float> dist;
    return [=](const float time) { return sigma * dist(generator); };
}

Series GenerateSamples(Function function, size_t N, float sample_freq) {
    std::vector<float> samples;
    for (size_t i = 0; i < N; ++i) {
        float t = static_cast<float>(i) / sample_freq;
        samples.push_back(function(t));
    }
    return samples;
}

}  // namespace cas