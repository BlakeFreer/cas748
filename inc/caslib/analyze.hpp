#pragma once

#include "series.hpp"

namespace cas {

float Mean(const Series& s);
float Variance(const Series& x);
float Covariance(const Series& x, const Series& y);

float CrossCorrelation(const Series& s1, const Series& s2);

}  // namespace cas