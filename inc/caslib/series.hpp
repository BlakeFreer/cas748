#pragma once

#include <vector>

namespace cas {

using Series = std::vector<float>;

Series operator+(const Series& x, const Series& y);
Series operator-(const Series& x, const Series& y);
Series operator*(const Series& x, const float scalar);
Series operator*(const float scalar, const Series& x);
Series operator-(const Series& x);

}  // namespace cas