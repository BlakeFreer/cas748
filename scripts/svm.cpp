#include <Eigen/Dense>
#include <iostream>

Eigen::VectorXd SupportVectorMachine(const Eigen::ArrayXXd& data,
                                     const Eigen::ArrayXd& labels);

int main() {
    // expected line y=-x-1
    Eigen::ArrayXXd data = {{
        {3, 4},
        {3, 0},
        {-5, 0},
        {-1, -4},
    }};
    Eigen::ArrayXd labels = {1, 1, 0, 0};
    std::cout << "SVM" << std::endl;
    return 0;
}

Eigen::VectorXd SupportVectorMachine(const Eigen::ArrayXXd& data,
                                     const Eigen::ArrayXd& labels) {}
