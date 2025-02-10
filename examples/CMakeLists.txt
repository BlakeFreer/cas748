add_executable(samples samples.cpp)
target_link_libraries(samples PRIVATE caslib)

add_executable(wav wav.cpp)
target_link_libraries(wav PRIVATE caslib)