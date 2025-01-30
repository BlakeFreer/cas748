#include "caslib/io.hpp"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "caslib/series.hpp"

namespace cas {

std::string PrettyPrint(const Series& s) {
    std::stringstream o;

    const int kMaxLines = 10;

    if (s.Size() <= kMaxLines) {
        for (size_t i = 0; i < s.Size(); i++) {
            o << s[i] << std::endl;
        }
    } else {
        int display = kMaxLines / 2;
        for (size_t i = 0; i < display; i++) {
            o << s[i] << std::endl;
        }
        o << "... (omitted " << s.Size() - kMaxLines + 1 << " values)"
          << std::endl;

        for (size_t i = s.Size() - display + 1; i < s.Size(); i++) {
            o << s[i] << std::endl;
        }
    }

    return o.str();
}

Series Load(const std::string& filename) {
    assert(std::filesystem::exists(filename));
    std::ifstream file(filename);
    std::string line;

    std::vector<float> data;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        data.push_back(std::stof(line));
    }

    return Series{data};
}

std::vector<Series> LoadSeries(const std::string& filename) {
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

void Save(const Series& series, const std::string& filename) {
    std::ofstream file(filename);
    file << series;
    file.close();
}

}  // namespace cas