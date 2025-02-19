cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

project(caslib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(caslib)
target_include_directories(caslib PUBLIC inc)
add_subdirectory(src)

# stb
target_include_directories(caslib PUBLIC third-party/stb)

# libsndfile
find_library(LIBSNDFILE NAMES libsndfile PATHS third-party/libsndfile/build)
target_link_libraries(caslib PUBLIC ${LIBSNDFILE})
target_include_directories(caslib PUBLIC third-party/libsndfile/include)

# Add the compiler's library and include paths to CMake search paths
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH};${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}")
set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH};${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
target_include_directories(caslib PUBLIC ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

# Find FFTW using the extended search paths
find_library(FFTW_LIB fftw3)
find_path(FFTW_INCLUDE_DIR fftw3.h)

if(FFTW_LIB AND FFTW_INCLUDE_DIR)
    message(STATUS "Found FFTW library: ${FFTW_LIB}")
    message(STATUS "Found FFTW include directory: ${FFTW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "FFTW was not found on your system!")
endif()

# Eigen
# find_library(EIGEN_LIB Eigen)
# target_link_libraries(caslib PUBLIC ${EIGEN_LIB})
target_link_libraries(caslib PUBLIC ${FFTW_LIB})
target_include_directories(caslib PUBLIC ${FFTW_INCLUDE_DIR})

add_subdirectory(test)
add_subdirectory(assignments)
add_subdirectory(scripts)