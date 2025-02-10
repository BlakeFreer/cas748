#pragma once

#include "caslib/series.hpp"

namespace cas {

struct AudioFile {
    int sample_rate;
    std::vector<Series> channels;
};

}  // namespace cas