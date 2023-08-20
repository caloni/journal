cmake_minimum_required(VERSION 3.21.1)

project(merge)

add_executable(merge
merge.cpp
../scripts/removeyaml.awk
)
target_compile_features(merge PRIVATE cxx_std_17)

install(TARGETS
merge
DESTINATION C:/Users/caloni/blog/scripts
)

