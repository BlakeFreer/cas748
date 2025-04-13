#include <Eigen/Core>
#include <filesystem>
#include <iostream>

#include "CLI/CLI.hpp"
#include "caslib/analyze.hpp"
#include "caslib/csv.hpp"

namespace Eigen {
template <class T>
void swap(T&& a, T&& b) {  // needed for std::sort
    a.swap(b);
}
}  // namespace Eigen

class ColumnLogger {
public:
    ColumnLogger(std::string filename);
    ~ColumnLogger();

    void AddColumn(std::string header, Eigen::ArrayXd column);
    void Close();
    std::string filename() const;

private:
    std::string filename_;
    std::vector<std::string> headers;
    std::vector<Eigen::ArrayXd> columns;
    bool closed_ = false;
};

Eigen::ArrayXd ExtractPeriodic(const Eigen::ArrayXd& data, int period);
int ChoosePeriodicity(Eigen::ArrayXd training_data);

int main(int argc, char* argv[]) {
    CLI::App cli;

    std::filesystem::path csv_file;
    cli.add_option("csv", csv_file, "CSV file to analyze")
        ->required()
        ->check(CLI::ExistingFile);

    int trend_degree, periodicity, predict_length, test_points;
    cli.add_option("-D", trend_degree, "Trend degree")->default_val(3);
    cli.add_option("-P", periodicity,
                   "Folding periodicity. If omitted, will choose the optimal.")
        ->default_val(-1);
    cli.add_option("-L", predict_length, "Linear Predictor Length")->required();
    cli.add_option("-T", test_points, "Length of test points at tail")
        ->default_val(20);

    CLI11_PARSE(cli, argc, argv);

    ColumnLogger csv{"out/prediction.csv"};

    /***************************************************************
        Read data
    ***************************************************************/
    Eigen::ArrayXXd timedata = cas::LoadCSV(csv_file, 0);

    csv.AddColumn("Time", timedata.col(0));
    csv.AddColumn("Actual", timedata.col(1));

    auto train = Eigen::seqN(0, timedata.rows() - test_points);
    int N_train = timedata(train, Eigen::all).rows();
    std::cout << "Training data size: " << N_train << std::endl;

    auto test = Eigen::seq(N_train, Eigen::last);
    int N_test = timedata(test, Eigen::all).rows();
    std::cout << "Test data size: " << N_test << std::endl;

    /***************************************************************
        Remove Trend
    ***************************************************************/
    Eigen::ArrayXd poly_fit =
        cas::PolynomialFit(trend_degree, timedata(train, Eigen::all));

    std::cout << "Polynomial trend coeffs:" << std::endl;
    std::cout << poly_fit.transpose() << std::endl;

    Eigen::ArrayXd trend = Eigen::ArrayXd::Zero(timedata.rows());
    for (int i = 0; i <= trend_degree; i++) {
        trend += poly_fit[i] * timedata.col(0).pow(i);
    }

    timedata.col(1) -= trend;
    csv.AddColumn("Trend", trend);
    csv.AddColumn("Actual - Trend", timedata.col(1));

    /***************************************************************
        Remove Periodicity with Folding
    ***************************************************************/
    if (periodicity == -1) {
        // if no periodicity was provided, then search for the p which best
        // explains the variance in the data
        periodicity = ChoosePeriodicity(timedata.col(1)(train));
    }

    std::cout << "Folding with period " << periodicity << std::endl;

    // Set every p^th element to the mean of the p-spaced training data
    Eigen::ArrayXd periodic = Eigen::ArrayXd::Zero(timedata.rows());
    for (int i = 0; i < periodicity; i++) {
        auto seq = Eigen::seq(i, Eigen::last, periodicity);
        periodic(seq) = timedata.col(1)(train)(seq).mean();
    }

    timedata.col(1) -= periodic;
    csv.AddColumn("Periodicity", periodic);
    csv.AddColumn("Actual - Trend - Periodicity", timedata.col(1));

    /***************************************************************
        Compute Autocorrelation
    ***************************************************************/
    Eigen::ArrayXd gamma =
        cas::Autocorrelation(timedata.col(1)(train)) / N_train;
    int a0 = gamma.size() / 2;

    ColumnLogger gamma_table{"out/autocorrelation.csv"};
    gamma_table.AddColumn("h",
                          Eigen::ArrayXd::LinSpaced(gamma.size(), -a0, a0));
    gamma_table.AddColumn("gamma(h)", gamma);
    gamma_table.Close();

    /***************************************************************
        Compute optimal predictor with LLS
    ***************************************************************/
    int autocor_train_len = 200;
    assert(autocor_train_len < N_train - predict_length);

    Eigen::MatrixXd X(autocor_train_len, predict_length);
    std::cout << "Matrix X size: " << X.rows() << "x" << X.cols() << std::endl;
    for (int i = 0; i < autocor_train_len; i++) {
        X.row(i) = timedata.col(1)(train)(Eigen::seqN(
            N_train - predict_length - autocor_train_len + i, predict_length));
    }

    Eigen::VectorXd Y = timedata.col(1)(train)(
        Eigen::seq(N_train - autocor_train_len, Eigen::last));
    std::cout << "Size of Y: " << Y.size() << std::endl;
    assert(X.rows() == Y.size());

    Eigen::MatrixXd ata = X.transpose() * X;
    Eigen::VectorXd predictor =
        ata.colPivHouseholderQr().solve(X.transpose() * Y);

    std::cout << "Optimal predictor of length " << predict_length << std::endl;
    std::cout << predictor.transpose() << std::endl;

    /***************************************************************
        Apply optimal predictor
    ***************************************************************/
    predictor.reverseInPlace();  // since coefficients are in -k direction

    Eigen::ArrayXd predicted = Eigen::ArrayXd::Zero(N_train + N_test);
    predicted(train) = timedata.col(1)(train);

    // Predict the remaining N_test points
    for (int k = 0; k < N_test; k++) {
        double pred = 0;
        int pred_idx = N_train + k;
        for (int i = 0; i < predict_length; i++) {
            pred += predictor[i] * predicted[pred_idx - i - 1];
        }
        predicted[pred_idx] = pred;
    }

    csv.AddColumn("Predicted", predicted);
    csv.Close();

    // Compute stats
    double var_before = cas::Variance(timedata.col(1)(test));
    double var_after = cas::Variance(timedata.col(1)(test) - predicted(test));

    std::cout << "Variance of stationary process:\n" << var_before << std::endl;
    std::cout << "Variance after prediction:\n" << var_after << std::endl;

    return 0;
}

Eigen::ArrayXd ExtractPeriodic(const Eigen::ArrayXd& data, int period) {
    Eigen::ArrayXd periodic(data.size());
    for (int i = 0; i < period; i++) {
        auto seq = Eigen::seq(i, Eigen::last, period);
        periodic(seq) = data(seq).mean();
    }
    return periodic;
}

int ChoosePeriodicity(Eigen::ArrayXd training_data) {
    int min_p = 20;                        // inclusive
    int max_p = training_data.size() / 2;  // exclusive

    std::vector<std::tuple<int, double>> scores;

    for (int p = min_p; p < max_p; p++) {
        Eigen::ArrayXd periodic = ExtractPeriodic(training_data, p);
        // low variance => good period choice
        double variance = cas::Variance(training_data - periodic);

        scores.push_back({p, variance});
    }

    Eigen::ArrayXXd sc(scores.size(), 2);
    for (int i = 0; i < scores.size(); i++) {
        sc(i, 0) = std::get<0>(scores[i]);
        sc(i, 1) = std::get<1>(scores[i]);
    }

    std::string period_filename = "out/folding_period.csv";
    cas::SaveCSV(period_filename, sc, {"Period", "Residual Variance"});

    // Sort periodicity_score rows by ascending variance
    std::sort(scores.begin(), scores.end(), [](auto const& a, auto const& b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    const int kDisplayBest = 5;
    std::cout << "Best folding periods:" << std::endl;
    std::cout << "Period | Residual RMS" << std::endl;
    for (int i = 0; i < kDisplayBest; i++) {
        auto [p, v] = scores[i];
        std::cout << p << " | " << v << std::endl;
    }
    std::cout << "Full results in " << period_filename << std::endl;

    // choose the best
    int periodicity = std::get<0>(scores[0]);
    std::cout << "Chose P=" << periodicity << std::endl;
    return periodicity;
}

ColumnLogger::ColumnLogger(std::string filename) : filename_(filename) {}

void ColumnLogger::AddColumn(std::string header, Eigen::ArrayXd column) {
    headers.push_back(header);
    columns.push_back(column);
}

void ColumnLogger::Close() {
    closed_ = true;
    if (columns.size() == 0) {
        return;
    }

    int rows = columns[0].size();

    Eigen::ArrayXXd table(rows, columns.size());

    for (int i = 0; i < columns.size(); i++) {
        Eigen::ArrayXd col = columns[i];
        if (col.size() != rows) {
            std::cerr << "Column \"" << headers[i] << "\" has length "
                      << col.size() << " whereas \"" << headers[0]
                      << "\" has length " << rows << std::endl;
            return;
        }

        table.col(i) = col;
    }

    cas::SaveCSV(filename_, table, headers);
}

ColumnLogger::~ColumnLogger() {
    if (!closed_) {
        Close();
    }
}

std::string ColumnLogger::filename() const {
    return filename_;
}
