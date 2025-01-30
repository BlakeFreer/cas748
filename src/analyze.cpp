#include "caslib/analyze.hpp"

#include <math.h>

#include <cassert>

namespace cas {

float Mean(const Series& s) {
    float total = 0;
    for (size_t i = 0; i < s.Size(); i++) {
        total += s[i];
    }
    return total / s.Size();
}

float Variance(const Series& x) {
    return Covariance(x, x);
}

float Covariance(const Series& x, const Series& y) {
    assert(x.Size() == y.Size());

    float x_bar = Mean(x);
    float y_bar = Mean(y);

    float covar = 0;
    for (size_t i = 0; i < x.Size(); i++) {
        covar += (x[i] - x_bar) * (y[i] - y_bar);
    }
    return covar / x.Size();
}

Series CrossCorrelation(const Series& x, const Series& y) {
    assert(x.Size() == y.Size());
    int size = x.Size();

    auto corel = Series::Zeros(size * 2 - 1);

    for (int lag = -size + 1; lag <= size - 1; lag++) {
        float covar = 0;

        if (lag <= 0) {
            for (int i = 0; i < size + lag; i++) {
                covar += x[i] * y[i - lag];
            }
        } else {
            for (int i = 0; i < size - lag; i++) {
                covar += x[i + lag] * y[i];
            }
        }

        corel[lag + size - 1] = covar;
    }

    return corel;
}

}  // namespace cas