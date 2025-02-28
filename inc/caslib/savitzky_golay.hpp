#pragma once

#include <Eigen/Core>

namespace cas::filter {

Eigen::ArrayXd SavitzkyGolay(int N, int degree);

}  // namespace cas::filter