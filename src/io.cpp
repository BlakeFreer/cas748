#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "caslib/io.hpp"
#include "caslib/series.hpp"

namespace cas {

std::string PrettyPrint(const Series& s) {
    std::stringstream o;

    const int kMaxLines = 10;

    if (s.size() <= kMaxLines) {
        for (auto f : s) {
            o << f << std::endl;
        }
    } else {
        int display = kMaxLines / 2;
        for (size_t i = 0; i < display; i++) {
            o << s[i] << std::endl;
        }
        o << "... (omitted " << s.size() - kMaxLines + 1 << " values)"
          << std::endl;

        for (size_t i = s.size() - display + 1; i < s.size(); i++) {
            o << s[i] << std::endl;
        }
    }

    return o.str();
}

Series Load(const std::string& filename) {
    assert(std::filesystem::exists(filename));
    std::ifstream file(filename);
    std::string line;

    Series output;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        output.push_back(std::stof(line));
    }
    return output;
}

std::vector<Series> LoadSeries(const std::string& filename) {
    assert(std::filesystem::exists(filename));

    std::ifstream file(filename);
    std::string line;
    std::string token;

    std::vector<Series> columns;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        int column = 0;
        while (std::getline(ss, token, ',')) {
            if (column >= columns.size()) {
                columns.push_back(Series{});
            }
            if (!token.empty()) {
                columns[column].push_back(std::stof(token));
            }
            column++;
        }
    }
    return columns;
}

void Save(const Series& series, const std::string& filename) {
    std::ofstream file(filename);
    file << series;
    file.close();
}

}  // namespace cas