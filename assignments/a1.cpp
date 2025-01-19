#include <iostream>

#include "caslib/functions.hpp"
#include "caslib/io.hpp"
#include "caslib/series.hpp"

using namespace cas;

int main() {
    Series loaded = Load("a1.data");
    std::cout << PrettyPrint(loaded);

    auto noisy = loaded + GenerateSamples(WhiteNoise(0.5), loaded.size(), 1);

    Save(noisy, "a1.data.noisy");

    return 0;
}