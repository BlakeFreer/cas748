#pragma once

#include "series.hpp"

namespace cas {

/// @brief Calculate the mean of a Series.
///
/// @param s
/// @return float
float Mean(const Series& s);

/// @brief Calculate the variance of a Series.
/// @note Uses population (not sample) variance.
///
/// @param x
/// @return float
float Variance(const Series& x);

/// @brief Calculate the covariance of two Series.
///
/// @param x
/// @param y
/// @return float
float Covariance(const Series& x, const Series& y);

/// @brief Calculate the Cross Correlation of two Series at all time lags.
/// @details Slides `x`, starting with the `x[0]` aligned with `y[last]` until
/// `x[last]` is aligned with `y[0]`.
///
/// @param x
/// @param y
/// @return Series
Series CrossCorrelation(const Series& x, const Series& y);

}  // namespace cas