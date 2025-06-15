cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(testjournal
  testconfig.cpp
  testjournal.cpp
  testmain.cpp
)
target_link_libraries(testjournal PRIVATE libjournal)
target_include_directories(testjournal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME RunUnitTests COMMAND testjournal)

