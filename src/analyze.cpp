#include "caslib/analyze.hpp"

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <map>
#include <vector>

namespace cas {

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

    // vote over the first k elements
    int max_vote = 0;
    int best_label;
    std::map<int, int> count;

    for (int i = 0; i < k; i++) {
        int label = distances[i].first;
        int votes = ++count[label];
        if (votes > max_vote) {
            best_label = label;
            max_vote = votes;
        }
    }

    return best_label;
}

}  // namespace cas