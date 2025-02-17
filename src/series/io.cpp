#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "caslib/series.hpp"

namespace cas {

std::string Series::PrettyPrint() const {
    std::stringstream o;

    const int kMaxLines = 10;

    if (size() <= kMaxLines) {
        for (size_t i = 0; i < size(); i++) {
            o << (*this)[i] << std::endl;
        }
    } else {
        int display = kMaxLines / 2;
        for (size_t i = 0; i < display; i++) {
            o << (*this)[i] << std::endl;
        }
        o << "... (omitted " << size() - kMaxLines + 1 << " values)"
          << std::endl;

        for (size_t i = size() - display + 1; i < size(); i++) {
            o << (*this)[i] << std::endl;
        }
    }

    return o.str();
}

Series Series::Load(const std::string& filename) {
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

void Series::Save(const std::string& filename) const {
    std::ofstream file(filename);
    file << (*this);
    file.close();
}

}  // namespace cas