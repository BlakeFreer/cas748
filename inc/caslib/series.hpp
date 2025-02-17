#pragma once

#include <functional>
#include <memory>
#include <ostream>
#include <vector>

#include "itertools.hpp"

namespace cas {

class Series {
public:
    // Constructors
    Series(size_t size = 0);
    Series(std::shared_ptr<float[]> data, size_t size);

    Series(std::initializer_list<float> values);
    explicit Series(std::vector<float> data);

    // Rule of 5
    Series(const Series& other);
    Series(Series&& other) noexcept;
    Series& operator=(Series other);

    friend void swap(Series& x, Series& y) noexcept;

    // Create new Series
    Series Clone() const;
    Series Apply(const std::function<float(float)>& func) const;
    static Series Zeros(size_t size);
    static Series Ones(size_t size);

    // Accessors
    size_t size() const;
    int ssize() const;

    // Operators
    Series operator+(const Series& y) const;
    Series operator-(const Series& y) const;
    Series operator*(const Series& y) const;
    Series operator-() const;

    // Scalar Operators
    Series operator*(const float scalar) const;
    Series operator/(const float divisor) const;
    Series operator+(const float scalar) const;
    Series operator-(const float scalar) const;
    friend Series operator*(const float scalar, const Series& x);
    friend Series operator+(const float scalar, const Series& x);
    friend Series operator-(const float scalar, const Series& x);

    float& operator[](int idx);
    float operator[](int idx) const;

    friend std::ostream& operator<<(std::ostream& o, const Series& s);

    Iterator<float> begin();
    Iterator<const float> begin() const;
    Iterator<float> end();
    Iterator<const float> end() const;

    const Series SubSeries(int start_idx, int end_idx) const;

    // Statistics
    float sum() const;
    float mean() const;
    float variance() const;
    float norm_to_p(int p) const;
    float norm(int p) const;

    static float Covariance(const Series& x, const Series& y);

    // I/O
    std::string PrettyPrint() const;
    static Series Load(const std::string& filename);
    void Save(const std::string& filename) const;

private:
    int WrapIndex(int idx) const;

    size_t size_;
    std::shared_ptr<float[]> data_;
};

}  // namespace cas
