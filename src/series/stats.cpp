#include <cassert>

#include "caslib/series.hpp"

namespace cas {

float Series::sum() const {
    float total = 0;
    for (auto f : (*this)) {
        total += f;
    }
    return total;
}

float Series::mean() const {
    return sum() / size();
}

float Series::variance() const {
    return Series::Covariance(*this, *this);
}

float Series::Covariance(const Series& x, const Series& y) {
    return ((x - x.mean()) * (y - y.mean())).mean();
}

}  // namespace cas