cmake_minimum_required(VERSION 3.21.1)

project(publish)
add_subdirectory(awklib)

add_executable(merge
merge.cpp
../scripts/md2adoc.awk
)
target_compile_features(merge PRIVATE cxx_std_17)

target_include_directories(merge PRIVATE
awklib/include
)

target_link_libraries(merge PRIVATE
awklib
)

add_executable(publish
constants.h
publish.cpp
../scripts/adoc2html.awk
)
target_compile_features(publish PRIVATE cxx_std_17)

target_include_directories(publish PRIVATE
awklib/include
)

target_link_libraries(publish PRIVATE
awklib
)

add_library(awkblog SHARED
constants.h
publish.cpp
awkblog.def
gawkapi.h
../scripts/adoc2html.awk
)
target_compile_features(awkblog PRIVATE cxx_std_17)

target_include_directories(awkblog PRIVATE
awklib/include
)

target_link_libraries(awkblog PRIVATE
awklib
)


install(TARGETS
merge
publish
awkblog
DESTINATION C:/Users/caloni/blog/scripts
)

