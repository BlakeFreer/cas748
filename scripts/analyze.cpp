#include "caslib/analyze.hpp"

#include <fftw3.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include "CLI/CLI.hpp"
#include "caslib/csv.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    CLI::App cli;

    fs::path csv_file;
    cli.add_option("csv", csv_file, "CSV file to analyze")
        ->required()
        ->check(CLI::ExistingFile);

    fs::path out_dir;
    cli.add_option("-o", out_dir, "Output directory")->required();

    bool csv_header = false;
    cli.add_flag("--header", csv_header,
                 "CSV has a header that should be ignored.");

    int column = 1;  // 1-indexed
    cli.add_option("-c,'--column", column, "CSV column to use.");

    CLI11_PARSE(cli, argc, argv);

    fs::path data_copy, auto_cor, fft_file;

    fs::create_directories(out_dir);

    /***************************************************************
        Load CSV and select column
    ***************************************************************/
    Eigen::ArrayXXd csv = cas::LoadCSV(csv_file, csv_header ? 1 : 0);
    std::cout << "Loaded " << csv_file << " which has " << csv.cols()
              << " columns." << std::endl;
    if (column < 1 || column > csv.cols()) {
        std::cerr << "Column " << column << " does not exist.";
        exit(1);
    }
    std::cout << "Using column " << column << "." << std::endl;
    Eigen::ArrayXd data = csv.col(column - 1);

    const int N = data.size();
    cas::SaveCSV(data_copy = out_dir / "data.csv", data);

    /***************************************************************
        Autocorrelation
    ***************************************************************/
    Eigen::ArrayXXd ac_table(2 * data.size() - 1, 2);
    ac_table.col(0) = Eigen::ArrayXd::LinSpaced(
        ac_table.rows(), -data.size() + 1, data.size() - 1),
    ac_table.col(1) = cas::CrossCorrelation(data, data);
    cas::SaveCSV(auto_cor = out_dir / "auto_correlation.csv", ac_table,
                 {"Lag", "Autocorrelation"});

    /***************************************************************
        Fourier Transform
    ***************************************************************/
    fftw_complex* fft = fftw_alloc_complex(N);
    fftw_plan fft_plan =
        fftw_plan_dft_r2c_1d(N, data.data(), fft, FFTW_ESTIMATE);

    fftw_execute(fft_plan);
    fftw_free(fft_plan);

    const int BINS = N / 2 + 1;
    Eigen::ArrayXcd fft_arr = Eigen::Map<Eigen::ArrayXcd>(
        reinterpret_cast<std::complex<double>*>(fft), BINS);
    Eigen::ArrayXXd fft_cols(BINS, 5);
    fft_cols.col(0) = Eigen::ArrayXd::LinSpaced(BINS, 0, BINS - 1);
    fft_cols.col(1) = fft_arr.real();
    fft_cols.col(2) = fft_arr.imag();
    fft_cols.col(3) = fft_arr.abs();
    fft_cols.col(4) = fft_arr.arg();

    cas::SaveCSV(fft_file = out_dir / "fft.csv", fft_cols,
                 {"Bin", "Real", "Imag", "Abs", "Arg"});

    fftw_free(fft);

    /***************************************************************
        Create GNUPLOT Script
    ***************************************************************/
    std::ofstream gplot(out_dir / "plot.gnuplot");
    gplot << std::format(
        R"(set datafile separator ",";
set title "Time Series Input";
set autoscale xy;
plot "{}" u 0:1 with l ls 1;
pause -1 "[ENTER] to continue";

set title "Fourier Transform";
set autoscale xy;
set xlabel "Bin";
plot "{}" u 1:2 with l ls 6 t columnhead,\
     "" u 1:3 with l ls 7 t columnhead,\
     "" u 1:4 with l ls 8 t columnhead;
pause -1 "[ENTER] to continue";

set title "Autocorrelation";
set autoscale xy;
set xlabel "Lag";
plot "{}" u 1:2 with lp ls 1 pt 7 t columnhead;
pause -1 "[ENTER] to continue";)",
        data_copy.generic_string(), fft_file.generic_string(),
        auto_cor.generic_string());
    gplot.close();

    std::cout << "Output saved to " << out_dir.string() << std::endl;
    return 0;
}