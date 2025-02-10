cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

project(caslib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(caslib)
target_include_directories(caslib PUBLIC inc)
add_subdirectory(src)
add_subdirectory(examples)

# libsndfile
find_library(LIBSNDFILE NAMES libsndfile PATHS third-party/libsndfile/build)
target_link_libraries(caslib PUBLIC ${LIBSNDFILE})
target_include_directories(caslib PUBLIC third-party/libsndfile/include)

set(GTEST_PATH "third-party/googletest")

if(EXISTS ${GTEST_PATH}/CMakeLists.txt)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(${GTEST_PATH})
    add_subdirectory(test)
else()
    message(WARNING "Skipping tests as GoogleTest is not installed.")
endif()