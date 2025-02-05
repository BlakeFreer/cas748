#include <cassert>
#include <cmath>

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

float Series::norm_to_p(int p) const {
    return this->Apply([&](auto f) { return std::pow(f, p); }).sum();
}

float Series::norm(int p) const {
    return std::pow(this->norm_to_p(p), 1.0 / p);
}

float Series::Covariance(const Series& x, const Series& y) {
    return ((x - x.mean()) * (y - y.mean())).mean();
}

}  // namespace cas