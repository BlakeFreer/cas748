#include <Eigen/Core>
#include <cmath>
#include <iostream>

#include "caslib/analyze.hpp"
#include "caslib/csv.hpp"

const double PI = 3.14159265358979323846;

Eigen::ArrayXd data_sin(int N, double amplitude, double freq, double phase,
                        double fs) {
    Eigen::ArrayXd sample_times = Eigen::ArrayXd::LinSpaced(N, 0, N - 1) / fs;
    return sample_times.unaryExpr([&](double t) {
        return amplitude * std::sin(2. * PI * freq * t + phase);
    });
}

int main() {
    /***************************************************************
        Generate sine wave
    ***************************************************************/
    double freq = 4;
    double amplitude = 2;
    double phase = PI / 4;
    double freq_sample = 32;
    std::cout << "Sine Wave\n"
              << "- Amplitude: " << amplitude << "\n"
              << "- Frequency: " << freq << " Hz\n"
              << "- Phase: " << phase << " rad\n"
              << "- Sample Freq: " << freq_sample << " Hz" << std::endl;

    Eigen::ArrayXd sine = data_sin(16, amplitude, freq, phase, freq_sample);
    std::cout << sine << std::endl;

    /***************************************************************
        Add white noise
    ***************************************************************/
    double noise_ampl = 0.5;
    std::cout << "Adding white noise with amplitude " << noise_ampl
              << std::endl;

    Eigen::ArrayXd sine_noisy =
        sine + Eigen::ArrayXd::Random(sine.size()) * noise_ampl;
    std::cout << sine_noisy << std::endl;

    /***************************************************************
        Cross Correlation
    ***************************************************************/
    std::cout << "CrossCorrelation of the clean and noisy signals."
              << std::endl;
    std::cout << cas::CrossCorrelation(sine, sine_noisy) << std::endl;

    /***************************************************************
        Save the clean and noisy signals
    ***************************************************************/
    std::string outfile = "signal.out";
    Eigen::ArrayXXd data(sine.size(), 2);
    data.col(0) = sine;
    data.col(1) = sine_noisy;

    cas::SaveCSV(outfile, data);
    std::cout << "Clean and noisy signals saved to " << outfile << std::endl;

    /***************************************************************
        Read the file back in
    ***************************************************************/
    Eigen::ArrayXXd loaded = cas::LoadCSV("signal.out");
    if ((loaded - data).abs().maxCoeff() < 0.0001) {
        std::cout << "CSV loaded data is equal to saved." << std::endl;
    } else {
        std::cout << "CSV loaded data is different! Uh oh!" << std::endl;
    }

    return 0;
}