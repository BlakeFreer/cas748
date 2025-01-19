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

    return 0;
}