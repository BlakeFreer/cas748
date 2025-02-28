#include <Eigen/Dense>

namespace cas::filter {

Eigen::ArrayXd SavitzkyGolay(int N, int degree) {
    if (N % 2 != 1) {
        throw std::invalid_argument("N must be odd (Savitzky-Golay).");
    }

    int range = N / 2;
    Eigen::ArrayXXd X =
        Eigen::ArrayXd::LinSpaced(N, -range, range).replicate(1, degree + 1);

    Eigen::ArrayXXd Pow = Eigen::ArrayXd::LinSpaced(degree + 1, 0, degree)
                              .transpose()
                              .replicate(N, 1);

    Eigen::MatrixXd A = X.pow(Pow);

    // filter is first row of normal equation solution
    Eigen::ArrayXd filter =
        ((A.transpose() * A).inverse() * A.transpose()).row(0);

    return filter;
}

}  // namespace cas::filter