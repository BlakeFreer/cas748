#include <cassert>
#include <ostream>

#include "caslib/series.hpp"

namespace cas {

float& Series::operator[](int idx) {
    size_t i = WrapIndex(idx);
    assert(0 <= i && i < size());

    return data_[i * stride_];
}
float Series::operator[](int idx) const {
    size_t i = WrapIndex(idx);
    assert(0 <= i && i < size());

    return data_[i * stride_];
}

Series Series::operator+(const Series& y) const {
    assert(this->size_ == y.size_);

    Series out = Clone();
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] += y[i];
    }
    return out;
}

Series Series::operator*(const Series& y) const {
    assert(this->size() == y.size());

    Series out = Clone();
    for (size_t i = 0; i < out.size(); i++) {
        out[i] *= y[i];
    }
    return out;
}

Series Series::operator-() const {
    return this->Apply([&](auto f) { return -f; });
}

Series Series::operator-(const Series& y) const {
    assert(size() == y.size());

    return *this + -y;
}

Series Series::operator+(const float scalar) const {
    return this->Apply([&](auto f) { return f + scalar; });
}
Series operator+(const float scalar, const Series& x) {
    return x + scalar;
}

Series Series::operator-(const float scalar) const {
    return this->Apply([&](auto f) { return f - scalar; });
}
Series operator-(const float scalar, const Series& x) {
    return x.Apply([&](auto f) { return scalar - f; });
}

Series Series::operator*(const float scalar) const {
    return this->Apply([&](auto f) { return f * scalar; });
}

Series operator*(const float scalar, const Series& x) {
    return x * scalar;
}
Series Series::operator/(const float divisor) const {
    return this->Apply([&](auto f) { return f / divisor; });
}

std::ostream& operator<<(std::ostream& o, const Series& x) {
    for (auto f : x) {
        o << f << std::endl;
    }
    return o;
}

}  // namespace cas