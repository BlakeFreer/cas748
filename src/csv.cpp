#include "caslib/csv.hpp"

#include <cassert>
#include <cmath>
#include <fstream>

namespace cas {

void SaveCSV(std::string filename, const Eigen::ArrayXXd& array,
             std::string delimiter, int precision) {
    std::ofstream of(filename);

    if (!of.is_open()) {
        throw std::runtime_error("Failed to open " + filename +
                                 " for writing.");
    }

    auto fmt =
        Eigen::IOFormat(precision, Eigen::DontAlignCols, delimiter, "\n");
    of << array.format(fmt);
}

namespace {
bool isvaluechar(char c) {
    return std::isdigit(c) || c == '.' || c == '-';
}
void trim_token(std::string& s) {
    // strip non-digit leading and trailing characters
    // in particular, converts "123" to 123

    while (!s.empty() && !isvaluechar(s.front())) {
        s.erase(0, 1);
    }
    while (!s.empty() && !isvaluechar(s.back())) {
        s.pop_back();
    }
}
}  // namespace

Eigen::ArrayXXd LoadCSV(std::string filename, char delimiter) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<std::vector<double>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            trim_token(token);

            if (token.empty()) {
                row.push_back(std::nan(0));
            } else {
                row.push_back(std::stod(token));
            }
        }
        data.push_back(row);
    }

    file.close();

    if (data.empty()) return Eigen::ArrayXXd(0, 0);

    size_t rows = data.size();
    size_t cols = data[0].size();
    Eigen::ArrayXXd table(rows, cols);

    for (int r = 0; r < rows; r++) {
        if (data[r].size() != cols) {
            throw std::runtime_error("Inconsistent row sizes.");
        }
        for (int c = 0; c < cols; c++) {
            table(r, c) = data[r][c];
        }
    }
    return table;
}
}  // namespace cas