#pragma once

#include <string>

#include "caslib/series.hpp"

namespace cas {

std::string PrettyPrint(const Series& s);
Series Load(const std::string& filename);
std::vector<Series> LoadSeries(const std::string& filename);
void Save(const Series& series, const std::string& filename);

}  // namespace cas