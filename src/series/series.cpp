#include "caslib/series.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

namespace cas {

Series::Series(size_t size) : size_(size), data_(new float[size_]()) {}

Series::Series(std::shared_ptr<float[]> data, size_t size)
    : size_(size), data_(data) {}

Series::Series(std::vector<float> vector)
    : size_(vector.size()), data_(size_ ? new float[size_]() : nullptr) {
    // can we take over the vector memory instead of copy it?
    for (size_t i = 0; i < size(); i++) {
        (*this)[i] = vector[i];
    }
}

// Copy Constructor
Series::Series(const Series& other)
    : size_(other.size_), data_(size_ ? new float[size_]() : nullptr) {
    std::memmove(data_.get(), other.data_.get(), size() * sizeof(float));
}

// Move Constructor
Series::Series(Series&& other) noexcept : size_(0), data_(nullptr) {
    swap(*this, other);
}

// Assignment
Series& Series::operator=(Series other) {
    swap(*this, other);
    return *this;
}

void swap(Series& x, Series& y) noexcept {
    using std::swap;
    swap(x.size_, y.size_);
    swap(x.data_, y.data_);
}

Series Series::Zeros(size_t size) {
    return Series{size};
}

Series Series::Ones(size_t size) {
    Series s{size};

    for (int i = 0; i < size; i++) {
        s[i] = 1;
    }
    return s;
}

size_t Series::size() const {
    return size_;
}

int Series::ssize() const {
    return static_cast<int>(size_);
}

Series Series::Clone() const {
    return Series{*this};
}
Series Series::Apply(const std::function<float(float)>& func) const {
    Series out = Clone();
    for (auto& f : out) {
        f = func(f);
    }
    return out;
}

Iterator<float> Series::begin() {
    return Iterator{data_.get()};
}
Iterator<float> Series::end() {
    return Iterator{data_.get() + size()};
}

Iterator<const float> Series::begin() const {
    return Iterator<const float>{data_.get()};
}
Iterator<const float> Series::end() const {
    return Iterator<const float>{data_.get() + size()};
}

const Series Series::SubSeries(int start_idx, int end_idx) const {
    start_idx = WrapIndex(start_idx);
    end_idx = WrapIndex(end_idx);

    assert(start_idx <= end_idx);
    assert(end_idx <= ssize());

    return Series{{data_, data_.get() + start_idx},
                  static_cast<size_t>(end_idx - start_idx)};
}

int Series::WrapIndex(int idx) const {
    if (idx < 0) {
        return ssize() + idx;
    }
    return idx;
}

}  // namespace  cas