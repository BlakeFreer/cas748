// Equations from
// https://numpy.org/doc/stable/reference/routines.window.html

#include "caslib/windows.hpp"

#include <cmath>

#include "caslib/series.hpp"

namespace cas::window {

Series Hanning(int N) {
    const float omega = 2 * std::numbers::pi / (N - 1);
    Series w(N);
    for (int n = 0; n < N; n++) {
        float nf = static_cast<float>(n);
        w[n] = 0.5 - 0.5 * std::cos(omega * nf);
    }
    return w;
}

Series Hamming(int N) {
    const float omega = 2 * std::numbers::pi / (N - 1);
    Series w(N);
    for (int n = 0; n < N; n++) {
        float nf = static_cast<float>(n);
        w[n] = 0.54 - 0.46 * std::cos(omega * nf);
    }
    return w;
}

Series Blackman(int N) {
    Series w(N);
    for (int n = 0; n < N; n++) {
        float nf = static_cast<float>(n);
        w[n] = 0.42 - 0.5 * std::cos(2 * std::numbers::pi * nf / N) +
               0.08 * std::cos(4 * std::numbers::pi * nf / N);
    }
    return w;
}

Series Bartlett(int N) {
    const float x = (N - 1) / 2.f;
    Series w(N);
    for (int n = 0; n < N; n++) {
        float nf = static_cast<float>(n);
        w[n] = (x - std::abs(nf - x)) / x;
    }
    return w;
}

Series Rectangular(int N) {
    return Series::Ones(N) / N;
}

}  // namespace cas::window