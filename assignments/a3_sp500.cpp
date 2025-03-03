#include <Eigen/Core>
#include <filesystem>
#include <iostream>

#include "CLI/CLI.hpp"
#include "caslib/analyze.hpp"
#include "caslib/csv.hpp"
#include "caslib/savitzky_golay.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    CLI::App cli;

    fs::path csv_file;
    cli.add_option("csv", csv_file, "CSV file to analyze")
        ->required()
        ->check(CLI::ExistingFile);

    int savgol_n, savgol_d;
    cli.add_option("-N", savgol_n, "Trend SavGol size")->default_val(101);
    cli.add_option("-D", savgol_d, "Trend SavGol degree")->default_val(3);

    CLI11_PARSE(cli, argc, argv);

    // expect csv has NO header and data in 2nd column
    Eigen::ArrayXd data = cas::LoadCSV(csv_file, 0).col(1);
    std::cout << "Data size: " << data.size() << std::endl;

    Eigen::ArrayXd savgol = cas::filter::SavitzkyGolay(savgol_n, savgol_d);
    Eigen::ArrayXd trend = cas::ApplyFilter(data, savgol);

    // trim trend
    Eigen::ArrayXd trend_trim =
        trend(Eigen::seq(savgol_n / 2, Eigen::last - savgol_n / 2));
    std::cout << "Trend size: " << trend_trim.size() << std::endl;

    Eigen::ArrayXXd out(data.size(), 2);
    out.col(0) = data;
    out.col(1) = trend_trim;

    cas::SaveCSV("trend.csv", out);

    return 0;
}