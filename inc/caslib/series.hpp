#pragma once

#include <ostream>
#include <vector>

namespace cas {

class Series {
public:
    // Constructors
    Series(float* data, size_t size);
    Series(std::vector<float> data);
    Series Clone() const;

    static Series Zeros(size_t size);
    static Series Ones(size_t size);

    // Accessors
    size_t Size() const;

    // Operators
    Series operator+(const Series& y) const;
    Series operator-(const Series& y) const;
    Series operator*(const float scalar) const;

    friend Series operator*(const float scalar, const Series& x);
    Series operator/(const float divisor) const;
    Series operator-() const;
    float& operator[](int idx);
    float operator[](int idx) const;

    friend std::ostream& operator<<(std::ostream& o, const Series& s);

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = float;
        using pointer = float*;
        using reference = float&;

        Iterator(float* p) : p_(p) {}
        Iterator(const Iterator& it) : p_(it.p_) {}
        Iterator& operator++() {
            ++p_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const Iterator& rhs) const {
            return p_ == rhs.p_;
        }
        bool operator!=(const Iterator& rhs) const {
            return p_ != rhs.p_;
        }
        float& operator*() {
            return *p_;
        }

    private:
        float* p_;
    };

    Iterator begin();
    Iterator end();

private:
    size_t WrapIndex(int idx) const;

    float* data_;
    size_t size_;
};
}  // namespace cas
