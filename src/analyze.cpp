#include <math.h>

#include <cassert>

#include "caslib/analyze.hpp"

namespace cas {

float Mean(const Series& s) {
    float total = 0;
    for (auto si : s) {
        total += si;
    }
    return total / s.size();
}

float Variance(const Series& x) {
    return Covariance(x, x);
}

float Covariance(const Series& x, const Series& y) {
    assert(x.size() == y.size());

    float x_bar = Mean(x);
    float y_bar = Mean(y);

    float var = 0;
    for (size_t i = 0; i < x.size(); i++) {
        var += (x[i] - x_bar) * (y[i] - y_bar);
    }
    return var / x.size();
}

float CrossCorrelation(const Series& s1, const Series& s2) {
    return Covariance(s1, s2) / sqrtf(Variance(s1) * Variance(s2));
}

}  // namespace cas