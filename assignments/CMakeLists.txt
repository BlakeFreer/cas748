add_executable(basic a1_basic.cpp)
target_link_libraries(basic PRIVATE caslib)

add_executable(spectrogram a2_spectrogram.cpp)
target_link_libraries(spectrogram PRIVATE caslib)

add_executable(sp500 a3_sp500.cpp)
target_link_libraries(sp500 PRIVATE caslib CLI11::CLI11)
