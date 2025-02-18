#include <iterator>

namespace cas {

template <typename T>
class Iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    Iterator(T* p) : p_(p) {}
    Iterator(T* p, size_t stride) : p_(p), stride_(stride) {}
    Iterator(const Iterator& it) : p_(it.p_), stride_(it.stride_) {}
    Iterator& operator++() {
        p_ = p_ + stride_;
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
    T& operator*() {
        return *p_;
    }

private:
    T* p_;
    size_t stride_ = 1;
};

}  // namespace cas