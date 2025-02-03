#include "caslib/series.hpp"

#include <cassert>
#include <cstring>
#include <ostream>

namespace cas {

Series::Series(float* data, size_t size) : data_(data), size_(size) {}

Series::Series(std::vector<float> vector) : size_(vector.size()) {
    // can we take over the vector memory instead of copy it?
    data_ = new float[size()];

    for (size_t i = 0; i < size(); i++) {
        (*this)[i] = vector[i];
    }
}

Series Series::Zeros(size_t size) {
    return Series{new float[size], size};
}

Series Series::Ones(size_t size) {
    Series s{new float[size], size};

    for (size_t i = 0; i < size; i++) {
        s[i] = 1;
    }
    return s;
}

size_t Series::size() const {
    return size_;
}

Series Series::Clone() const {
    float* cpy;
    std::memcpy(cpy, data_, size() * sizeof(float));
    return Series{cpy, size()};
}

float& Series::operator[](int idx) {
    size_t i = WrapIndex(idx);
    assert(0 <= i && i < size());

    return data_[i];
}
float Series::operator[](int idx) const {
    size_t i = WrapIndex(idx);
    assert(0 <= i && i < size());

    return data_[i];
}

Series Series::operator+(const Series& y) const {
    assert(this->size_ == y.size_);

    Series out = Clone();
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] += y[i];
    }
    return out;
}

Series Series::operator*(const float scalar) const {
    Series out = Clone();
    for (size_t i = 0; i < out.size(); i++) {
        out[i] *= scalar;
    }
    return out;
}

Series operator*(const float scalar, const Series& x) {
    return x * scalar;
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
    Series out = Clone();
    for (auto& f : out) {
        f = -f;
    }
    return out;
}

Series Series::operator-(const Series& y) const {
    assert(size() == y.size());

    return *this + -y;
}

Series Series::operator/(const float divisor) const {
    Series out = Clone();
    for (size_t i = 0; i < out.size(); i++) {
        out[i] /= divisor;
    }
    return out;
}

std::ostream& operator<<(std::ostream& o, const Series& x) {
    for (size_t i = 0; i < x.size(); i++) {
        o << x[i] << std::endl;
    }
    return o;
}

Series::Iterator Series::begin() {
    return Iterator{data_};
}
Series::Iterator Series::end() {
    return Iterator{data_ + size_};
}

size_t Series::WrapIndex(int idx) const {
    if (idx < 0) {
        return size_ + idx;
    }
    return idx;
}

}  // namespace  cas