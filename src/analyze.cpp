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

    float covar = 0;
    for (size_t i = 0; i < x.size(); i++) {
        covar += (x[i] - x_bar) * (y[i] - y_bar);
    }
    return covar / x.size();
}

Series CrossCorrelation(const Series& x, const Series& y) {
    assert(x.size() == y.size());
    int size = x.size();

    Series corel;

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

        corel.push_back(covar);
    }

    return corel;
}

}  // namespace cas