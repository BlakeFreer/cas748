#include <Eigen/Core>
#include <filesystem>
#include <iostream>

#include "CLI/CLI.hpp"
#include "caslib/csv.hpp"

using namespace std::numbers;
namespace fs = std::filesystem;

void Save(fs::path filename, const Eigen::ArrayXd& data) {
    cas::SaveCSV(filename, data);
    std::cout << filename.generic_string() << std::endl;
}

int main(int argc, char* argv[]) {
    CLI::App cli;

    int length = 256;
    cli.add_option("length", length, "Length of data to generated.")
        ->required();

    fs::path out_dir;
    cli.add_option("out-dir", out_dir, "Where to place files.")->required();

    CLI11_PARSE(cli, argc, argv);

    fs::create_directories(out_dir);

    const int N = length;
    std::cout << "Each file will have " << N << " datapoints." << std::endl;

    Save(out_dir / "linear.csv", Eigen::ArrayXd::LinSpaced(N, 0, N - 1) / N);

    Save(out_dir / "dc.csv", Eigen::ArrayXd::Constant(N, 1));

    Eigen::ArrayXd t_range = Eigen::ArrayXd::LinSpaced(N, 0, N - 1);
    Save(out_dir / "sine_per16.csv",
         t_range.unaryExpr([](double t) { return std::sin(2 * pi * t / 16); }));

    Save(out_dir / "whitenoise.csv", Eigen::ArrayXd::Random(N));

    Eigen::ArrayXd imp = Eigen::ArrayXd::Zero(N);
    imp[0] = 1;
    Save(out_dir / "impulse.csv", imp);

    Eigen::ArrayXd imp_delay = Eigen::ArrayXd::Zero(N);
    imp_delay[N / 2] = 1;
    Save(out_dir / "impulse_delayed.csv", imp_delay);

    return 0;
}