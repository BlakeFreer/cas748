#pragma once

#include "caslib/series.hpp"

namespace cas {

Series Hanning(int N);
Series Hamming(int N);
Series Blackman(int N);
Series Bartlett(int N);

}  // namespace cas