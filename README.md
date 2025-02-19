# caslib

Code for CAS 748.

## Usage

The main datatype is `Series` which represents a one-dimensional time series. `Series` can be added, subtracted and scaled.

## Compiling and Testing

```bash
git clone https://github.com/BlakeFreer/cas748.git --recurse-submodules
cmake -S. -Bbuild
cmake --build build
```

Tests will run during the build process.

This generates `build/libcaslib.a`. Compile your project against it.

```bash
g++ main.cpp -L build -l caslib -I inc
```

## Dependencies

These libraries must be installed on your system.

- Eigen3
- FFTW3

## Scripts

Scripts use `caslib` to accomplish a specific task.

### `scripts/analyze`

Given a CSV file, compute various transforms, such as the Fourier Transform and correlation.

It generates a `gnuplot` script to view the output.

```bash
./build/scripts/analyze example.csv -o output/example
gnuplot output/example/plot.gnuplot
```

If `example.csv` has column headers then pass `--header` to `./analyze` to skip it. If the data has multiple columns, use `--column X` to process the `X`th column (1-indexed).

### `scripts/make_data`

Generate several standard time series, like a sine wave, impulse, and white noise.

```bash
$ ./build/scripts/make_data 500 sample_data
Each file will have 500 datapoints.
sample_data/linear.csv
sample_data/dc.csv
sample_data/sine_per16.csv
sample_data/whitenoise.csv
sample_data/impulse.csv
sample_data/impulse_delayed.csv
```
