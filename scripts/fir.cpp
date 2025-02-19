#include <filesystem>
#include <iostream>

#include "caslib/csv.hpp"

namespace fs = std::filesystem;

struct Config {
    fs::path csv;

    Config(int argc, char* argv[]) {
        if (argc != 2) {
            std::cerr << "Usage: ./fir <csv-file>" << std::endl;
            exit(2);
        }

        csv = argv[1];
    }
};

double filter(double x0) {
    // 0-initialize memory
    static double x1 = 0;

    double y = (x0 + x1) / 2;

    // Update memory
    x1 = x0;

    return y;
}

int main(int argc, char* argv[]) {
    Config conf(argc, argv);

    Eigen::ArrayXd data = cas::LoadCSV(conf.csv);

    Eigen::ArrayXd filtered =
        data.unaryExpr([](double x) { return filter(x); });

    cas::SaveCSV("filtered.csv", filtered);

    return 0;
}