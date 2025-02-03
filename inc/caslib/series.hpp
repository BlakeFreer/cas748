#pragma once

#include <ostream>
#include <vector>

#include "itertools.hpp"

namespace cas {

class Series {
public:
    // Constructors
    Series(float* data, size_t size);
    Series(std::vector<float> data);

    template <typename... Args>
    Series(Args&&... args);

    Series Clone() const;

    static Series Zeros(size_t size);
    static Series Ones(size_t size);

    // Accessors
    size_t size() const;

    // Operators
    Series operator+(const Series& y) const;
    Series operator-(const Series& y) const;
    Series operator*(const float scalar) const;
    Series operator*(const Series& y) const;

    friend Series operator*(const float scalar, const Series& x);
    Series operator/(const float divisor) const;
    Series operator-() const;
    float& operator[](int idx);
    float operator[](int idx) const;

    friend std::ostream& operator<<(std::ostream& o, const Series& s);

    Iterator<float> begin();
    Iterator<float> end();

private:
    size_t WrapIndex(int idx) const;

    float* data_;
    size_t size_;
};

template <typename... Args>
Series::Series(Args&&... args) : size_(sizeof...(args)) {
    data_ = new float[size_]{static_cast<float>(args)...};
}

}  // namespace cas
