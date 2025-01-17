#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace cas {

using Function = std::function<float(float)>;

Function Sine(const float amplitude, const float frequency);
Function WhiteNoise(const float amplitude);
Function GaussianNoise(const float sigma);

std::vector<float> GenerateSamples(Function function, size_t N,
                                   float sample_freq);

}  // namespace cas