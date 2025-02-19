# caslib

Code for CAS 748.

## Usage

The main datatype is `Series` which represents a one-dimensional time series. `Series` can be added, subtracted and scaled.

## Compiling and Testing

To use `caslib` in another project, first build the library archive.

```bash
cmake -S. -Bbuild -G"Unix Makefiles"
cmake --build build
```

This generates `build/libcaslib.a`. Compile your project against it.

```bash
g++ main.cpp -L build -l caslib -I inc
```

## Dependencies

These libraries must be installed on your system.

- Eigen3
- FFTW3

### Testing

First clone the repo with submodules to install GoogleTest.

```bash
git clone https://github.com/BlakeFreer/cas748.git --recurse-submodules
```

Re-run the CMake configuration and build steps. The tests will run automatically.
