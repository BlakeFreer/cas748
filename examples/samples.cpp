#include <iostream>

#include "caslib/functions.hpp"

using namespace cas;

int main() {
    std::cout << "Generating 12 data points for different functions."
              << std::endl;

    std::vector<std::tuple<Function, std::string>> functions = {
        {Sine(1, 1), "Sine"},
        {WhiteNoise(1), "White Noise"},
        {GaussianNoise(1), "Gaussian Noise"},
    };

    for (auto [f, name] : functions) {
        std::cout << name << std::endl;

        auto samples = GenerateSamples(f, 12, 12);
        for (auto s : samples) {
            std::cout << s << std::endl;
        }
    }

    return 0;
}