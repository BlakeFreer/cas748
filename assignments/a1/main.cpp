#include <iostream>

#include "caslib/functions.hpp"
#include "caslib/series.hpp"

using namespace cas;

int main() {
    Series loaded = Series::Load("input.csv");
    std::cout << loaded.PrettyPrint();

    auto noisy = loaded + GenerateSamples(WhiteNoise(0.5), loaded.size(), 1);

    noisy.Save("noisy.csv.out");

    std::cout
        << "Output saved to `noise.csv.out`. "
        << "The data points have noise added to them relative to `input.csv`."
        << std::endl;

    return 0;
}