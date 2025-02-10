target_sources(caslib
    PRIVATE
    analyze.cpp
    functions.cpp
    io.cpp
)
add_subdirectory(series)
add_subdirectory(file)