#include "caslib/analyze.hpp"

#include <Eigen/Dense>

namespace cas {

Eigen::ArrayXd PolynomialFit(int degree, const Eigen::ArrayXd& y,
                             const Eigen::ArrayXd& x) {
    if (x.size() != y.size()) {
        throw std::runtime_error("x and y must have the same size.");
    }
    int N = x.size();
    Eigen::MatrixXd A(N, degree + 1);

    for (int i = 0; i <= degree; i++) {
        A.col(i) = x.pow(i);
    }

    Eigen::VectorXd coeffs =
        (A.transpose() * A).inverse() * A.transpose() * y.matrix();
    return coeffs.array();
}
}  // namespace cas