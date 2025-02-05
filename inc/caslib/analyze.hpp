#pragma once

#include "series.hpp"

namespace cas {

/// @brief Calculate the Cross Correlation of two Series at all time lags.
/// @details Slides `x`, starting with the `x[0]` aligned with `y[last]` until
/// `x[last]` is aligned with `y[0]`.
///
/// @param x
/// @param y
/// @return Series
Series CrossCorrelation(const Series& x, const Series& y);

Series CrossCovariance(const Series& x, const Series& y);

}  // namespace cas