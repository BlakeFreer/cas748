# caslib

Code for CAS 748. Also practicing C++, CMake, and GoogleTest.

## Usage

The main datatype is `Series` which represents a one-dimensional time series. `Series` can be added, subtracted and scaled.

## Compiling

To use `caslib` in another project, first build the library archive.

```bash
cmake -S. -Bbuild -G"Unix Makefiles"
cmake --build build
```

This generates `build/libcaslib.a`. Compile your project against it.

```bash
g++ main.cpp -L build -l caslib -I inc
```
