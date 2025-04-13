add_executable(basic a1_basic.cpp)
target_link_libraries(basic PRIVATE caslib)

add_executable(spectrogram a2_spectrogram.cpp)
target_link_libraries(spectrogram PRIVATE caslib)

add_executable(predict a3_predict.cpp)
target_link_libraries(predict PRIVATE caslib CLI11::CLI11)
