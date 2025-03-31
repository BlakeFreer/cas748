#pragma once

#include <Eigen/Core>

namespace cas {

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Correlation(const Eigen::ArrayBase<Derived>& x,
            const Eigen::ArrayBase<Derived>& y) {
    return (x * y.conjugate()).sum();
}

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Covariance(const Eigen::ArrayBase<Derived>& x,
           const Eigen::ArrayBase<Derived>& y) {
    return Correlation(x - x.mean(), y - y.mean()) / x.size();
}

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Variance(const Eigen::ArrayBase<Derived>& x) {
    return Covariance(x, x);
}

/// @brief Calculate the Cross Correlation of two Series at all time lags.
/// @details Slides `x`, starting with the `x[0]` aligned with `y[last]` until
/// `x[last]` is aligned with `y[0]`.
///
/// @param x
/// @param y
template <typename Derived>
Derived CrossCorrelation(const Eigen::ArrayBase<Derived>& x,
                         const Eigen::ArrayBase<Derived>& y) {
    Derived corel(x.size() * 2 - 1);

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
Derived Pad(const Eigen::ArrayBase<Derived>& signal, int count,
            typename Derived::Scalar constant = 0) {
    if (count < 0) {
        throw std::invalid_argument("Padding amount must be nonnegative");
    }
    Derived padded = Derived::Zero(signal.size() + 2 * count);
    padded += constant;
    padded(Eigen::seq(count, Eigen::last - count)) = signal;
    return padded;
}

/// @brief Apply a filter.
template <typename Derived>
Derived ApplyFilter(const Eigen::ArrayBase<Derived>& signal,
                    const Eigen::ArrayBase<Derived>& filter) {
    Derived padded = Pad(signal, filter.size() - 1);

    Derived out(signal.size() + filter.size() - 1);

    // NOT EFFICIENT - direct convolution
    for (int i = 0; i < out.size(); i++) {
        Eigen::ArrayXd sub = padded(Eigen::seq(i, i + filter.size() - 1));
        out[i] = (sub * filter.reverse()).sum();
    }
    return out;
}

Eigen::ArrayXd PolynomialFit(int degree, const Eigen::ArrayXd& y,
                             const Eigen::ArrayXd& x);

/// @brief Compute the new basis vectors with PCA.
/// The vectors are sorted by decreasing variance.
///
/// @param data NxD, N points of dimension D
/// @return Eigen::ArrayXXd
Eigen::ArrayXXd PrincipleComponentAnalysis(const Eigen::ArrayXXd& data);

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