#include "caslib/analyze.hpp"

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <map>
#include <vector>

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

Eigen::ArrayXXd PrincipleComponentAnalysis(const Eigen::ArrayXXd& data) {
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

Eigen::ArrayXXd SupportVectorMachine(const Eigen::ArrayXXd& data,
                                     const Eigen::ArrayXd& labels);

int KNearestNeighbours(const Eigen::ArrayXXd& data,
                       const Eigen::ArrayXi& labels,
                       const Eigen::ArrayXd& point, int k) {
    int N = data.rows();
    assert(data.cols() == point.size());
    assert(N == labels.size());
    assert(k > 0);

    // compute distances
    std::vector<std::pair<int, double>> distances;
    for (int i = 0; i < N; i++) {
        Eigen::VectorXd d = data.row(i) - point;
        distances.push_back({labels(i), d.squaredNorm()});
    }

    // sort labels
    std::sort(distances.begin(), distances.end(),
              [](auto i, auto j) { return i.second < j.second; });

    if (k == 1) return distances[0].first;

    // count repetitions in first k elements
    std::map<int, int> count;
    for (int i = 0; i < k; i++) {
        auto v = distances[i].first;
        if (count.contains(v)) {
            count[v]++;
        } else {
            count[v] = 1;
        }
    }

    // find most common label among k nearest neighbours
    int best_label;
    int max_vote = 0;
    for (auto [k, v] : count) {
        if (v > max_vote) {
            max_vote = v;
            best_label = k;
        }
    }
    return best_label;
}

}  // namespace cas