#pragma once

#include "series.hpp"

namespace cas {

float Mean(const Series& s);
float Variance(const Series& x);
float Covariance(const Series& x, const Series& y);

Series CrossCorrelation(const Series& x, const Series& y);

}  // namespace cas