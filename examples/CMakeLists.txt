add_executable(samples samples.cpp)
target_link_libraries(samples PRIVATE caslib)

add_executable(fourier fourier.cpp)
target_link_libraries(fourier PRIVATE caslib)
