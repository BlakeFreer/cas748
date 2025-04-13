#pragma once

#include <Eigen/Dense>

namespace cas {

template <typename T>
using _ArrayDyn = Eigen::Array<T, Eigen::Dynamic, 1>;

template <typename Derived>
typename Derived::Scalar Correlation(const Eigen::ArrayBase<Derived>& x,
                                     const Eigen::ArrayBase<Derived>& y) {
    return (x * y.conjugate()).sum();
}

template <typename Derived>
typename Derived::Scalar Covariance(const Eigen::ArrayBase<Derived>& x,
                                    const Eigen::ArrayBase<Derived>& y) {
    return Correlation(x - x.mean(), y - y.mean()) / x.size();
}
template <typename Derived>
typename Derived::Scalar Variance(const Eigen::ArrayBase<Derived>& x) {
    return Covariance(x, x);
}

/// @brief Calculate the Cross Correlation of two Series at all time lags.
/// @details Slides `x`, starting with the `x[0]` aligned with `y[last]` until
/// `x[last]` is aligned with `y[0]`.
///
/// @param x
/// @param y
template <typename D1, typename D2 = D1>
_ArrayDyn<typename D1::Scalar> CrossCorrelation(const Eigen::ArrayBase<D1>& x,
                                                const Eigen::ArrayBase<D2>& y) {
    using Array = _ArrayDyn<typename D1::Scalar>;
    Array corel(x.size() * 2 - 1);

    for (int lag = -x.size() + 1; lag <= x.size() - 1; lag++) {
        if (lag < 0) {  // x is delayed
            corel[lag + x.size() - 1] =
                Correlation(x(Eigen::seq(0, Eigen::last + lag)),
                            y(Eigen::seq(-lag, Eigen::last)));
        } else if (lag == 0) {
            corel[lag + x.size() - 1] = Correlation(x, y);
        } else {  // y is delayed
            corel[lag + x.size() - 1] =
                Correlation(x(Eigen::seq(lag, Eigen::last)),
                            y(Eigen::seq(0, Eigen::last - lag)));
        }
    }

    return corel;
}

template <typename Derived>
_ArrayDyn<typename Derived::Scalar> Autocorrelation(
    const Eigen::ArrayBase<Derived>& x) {
    return CrossCorrelation(x, x);
}

template <typename Derived>
_ArrayDyn<typename Derived::Scalar> Pad(const Eigen::ArrayBase<Derived>& signal,
                                        int count,
                                        typename Derived::Scalar constant = 0) {
    using Array = _ArrayDyn<typename Derived::Scalar>;

    if (count < 0) {
        throw std::invalid_argument("Padding amount must be nonnegative");
    }
    Array padded = Array::Zero(signal.size() + 2 * count);
    padded += constant;
    padded(Eigen::seq(count, Eigen::last - count)) = signal;
    return padded;
}

/// @brief Apply a filter.
template <typename D1, typename D2 = D1>
_ArrayDyn<typename D1::Scalar> ApplyFilter(const Eigen::ArrayBase<D1>& signal,
                                           const Eigen::ArrayBase<D2>& filter) {
    using Array = _ArrayDyn<typename D1::Scalar>;

    Array padded = Pad(signal, filter.size() - 1);
    Array out(signal.size() + filter.size() - 1);

    // NOT EFFICIENT - direct convolution
    for (int i = 0; i < out.size(); i++) {
        Array sub = padded(Eigen::seq(i, i + filter.size() - 1));
        out[i] = (sub * filter.reverse()).sum();
    }
    return out;
}

template <typename Derived>
Eigen::ArrayXd PolynomialFit(int degree, const Eigen::ArrayBase<Derived>& xy) {
    if (xy.cols() != 2) {
        throw std::invalid_argument("xy must have 2 columns");
    }

    int N = xy.rows();
    Eigen::MatrixXd A(N, degree + 1);

    for (int i = 0; i <= degree; i++) {
        A.col(i) = xy.col(0).pow(i);
    }

    Eigen::MatrixXd ATA = A.transpose() * A;
    // Eigen::VectorXd coeffs =
    //     ATA.colPivHouseholderQr().solve(A.transpose() * xy.col(1).matrix());

    Eigen::VectorXd coeffs =
        (A.transpose() * A).inverse() * A.transpose() * xy.col(1).matrix();

    return coeffs.array();
}

/// @brief Compute the new basis vectors with PCA.
/// The vectors are sorted by decreasing variance.
///
/// @param data NxD, N points of dimension D
/// @return Eigen::ArrayXXd
template <typename Derived>
Eigen::MatrixXd PrincipleComponentAnalysis(
    const Eigen::ArrayBase<Derived>& data) {
    Eigen::MatrixXd A = data.matrix().transpose() * data.matrix();
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(A);

    if (es.info() != Eigen::Success) {
        std::runtime_error("SelfAdjointEigenSolver failed in PCA");
    }

    Eigen::VectorXd eigenvalues = es.eigenvalues();
    Eigen::MatrixXd eigenvectors = es.eigenvectors();

    std::vector<std::pair<double, Eigen::VectorXd>> eigen_pairs;
    for (int i = 0; i < eigenvalues.size(); ++i) {
        eigen_pairs.emplace_back(eigenvalues(i), eigenvectors.col(i));
    }

    std::sort(eigen_pairs.begin(), eigen_pairs.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    for (int i = 0; i < eigenvalues.size(); ++i) {
        eigenvectors.col(i) = eigen_pairs[i].second;
    }
    return eigenvectors;
}

/// @brief Compute the optimal SVM separating hyperplane.
///
/// @param data NxD, N points of D dimensions
/// @note Assumes the data has zero mean
/// @param labels [-1, +1] for each point.
/// @return Eigen::ArrayXd w for f(x)=wT x
Eigen::ArrayXd SupportVectorMachine(const Eigen::ArrayXXd& data,
                                    const Eigen::ArrayXi& labels);

int KNearestNeighbours(const Eigen::ArrayXXd& data,
                       const Eigen::ArrayXi& labels,
                       const Eigen::ArrayXd& point, int k);

}  // namespace cas