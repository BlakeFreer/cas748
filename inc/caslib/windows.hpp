#pragma once

#include "caslib/series.hpp"

namespace cas::window {

Series Hanning(int N);
Series Hamming(int N);
Series Blackman(int N);
Series Bartlett(int N);
Series Rectangular(int N);

}  // namespace cas::window