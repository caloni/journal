cmake_minimum_required(VERSION 3.21.1)

project(publish)

add_executable(publish
publish.cpp
)
target_compile_features(publish PRIVATE cxx_std_17)

add_executable(merge
merge.cpp
)
target_compile_features(merge PRIVATE cxx_std_17)
