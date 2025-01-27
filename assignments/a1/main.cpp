#include <iostream>

#include "caslib/functions.hpp"
#include "caslib/io.hpp"
#include "caslib/series.hpp"

using namespace cas;

int main() {
    Series loaded = Load("input.csv");
    std::cout << PrettyPrint(loaded);

    auto noisy = loaded + GenerateSamples(WhiteNoise(0.5), loaded.size(), 1);

    Save(noisy, "noisy.csv.out");

    std::cout
        << "Output saved to `noise.csv.out`. "
        << "The data points have noise added to them relative to `noise.csv`."
        << std::endl;

    return 0;
}