#include <Eigen/Core>

namespace cas {

void SaveCSV(std::string filename, const Eigen::ArrayXXd& array,
             std::string delimiter = ",", int precision = Eigen::FullPrecision);
Eigen::ArrayXXd LoadCSV(std::string filename, char delimiter = ',');

}  // namespace cas