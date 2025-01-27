# Assignment 1

Write C / C++ code to do some elementary signal processing. GOAL: To get used to the coding approaches and build tools for time series analysis.

Functionality to implement:

1. All data is `float*` together with an `int` for the size.
2. Data generation. Generate a sine with amplitude and frequency (and sampling frequency) as a parameter.

    ```c++
    int data_sin(float* data, int size, float a, float f, float fs);
    ```

3. Read data from csv file. (I give you many of these).

    ```c++
    int read_data(const char* name, float* data);
    ```

4. Add noise:

    ```c++
    void add_noise(float* data, int size);
    ```

5. A procedure that, given two time series cross-correlation functions, returns an array. (This function is also used to compute the autocorrelation).

    ```c++
    float* cross_corel(float* data1, float* data2, int size);
    ```

## Submission

Show the code to MvM and email him a copy of it.
