#include <cassert>

#include "caslib/series.hpp"

namespace cas {

Series operator+(const Series& x, const Series& y) {
    assert(x.size() == y.size());

    Series output;
    for (size_t i = 0; i < x.size(); ++i) {
        output.push_back((x[i] + y[i]));
    }
    return output;
}

Series operator*(const Series& x, const float scalar) {
    Series output;
    for (auto xi : x) {
        output.push_back(xi * scalar);
    }
    return output;
}

Series operator*(const float scalar, const Series& x) {
    return x * scalar;
}

Series operator-(const Series& x) {
    Series output;
    for (auto xi : x) {
        output.push_back(-xi);
    }
    return output;
}

Series operator-(const Series& x, const Series& y) {
    assert(x.size() == y.size());

    return x + -y;
}

}  // namespace  cas