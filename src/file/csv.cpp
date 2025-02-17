#include <caslib/file/csv.hpp>
#include <caslib/series.hpp>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace cas {

std::vector<Series> LoadCSV(std::string filename) {
    assert(std::filesystem::exists(filename));

    std::ifstream file(filename);
    std::string line;
    std::string token;

    std::vector<std::vector<float>> columns;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        int column = 0;
        while (std::getline(ss, token, ',')) {
            if (column >= columns.size()) {
                columns.push_back(std::vector<float>{});
            }
            if (!token.empty()) {
                try {
                    columns[column].push_back(std::stof(token));
                } catch (std::invalid_argument e) {
                    std::cerr << "Could not parse " << token << std::endl;
                    columns[column].push_back(NAN);
                }
            }
            column++;
        }
    }

    std::vector<Series> dataframe;
    for (auto col : columns) {
        dataframe.push_back(Series{col});
    }
    return dataframe;
}

}  // namespace cas