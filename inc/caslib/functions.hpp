#pragma once

#include <cstddef>
#include <functional>

#include "series.hpp"

namespace cas {

using Function = std::function<float(float)>;

Function Sine(const float amplitude, const float frequency);
Function WhiteNoise(const float amplitude);
Function GaussianNoise(const float sigma);

Series GenerateSamples(Function function, size_t N, float sample_freq);

}  // namespace cas