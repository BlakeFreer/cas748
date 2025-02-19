#include <Eigen/Core>
#include <filesystem>
#include <vector>

namespace cas {

void SaveCSV(std::filesystem::path filename, const Eigen::ArrayXXd& array,
             std::vector<std::string> header = {}, std::string delimiter = ",",
             int precision = Eigen::FullPrecision);

Eigen::ArrayXXd LoadCSV(std::filesystem::path filename, int skip_lines = 0,
                        char delimiter = ',');

}  // namespace cas