cmake_minimum_required(VERSION 3.21.1)

project(publish)
add_subdirectory(awklib)

add_executable(merge
merge.cpp
)
target_compile_features(merge PRIVATE cxx_std_17)

target_include_directories(merge PRIVATE
awklib/include
)

target_link_libraries(merge PRIVATE
awklib
)

add_executable(publish
publish.cpp
)
target_compile_features(publish PRIVATE cxx_std_17)

install(TARGETS
merge
publish
DESTINATION C:/Users/caloni/blog/scripts
)

