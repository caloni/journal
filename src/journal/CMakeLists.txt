cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(journal
  LANGUAGES CXX)

#add_library(libjournal
  #README.md
  #journal.cpp
  #journal.h
  #underworld.cpp
  #underworld.h
#)

add_executable(journal
  main.cpp
)
target_link_libraries(journal PRIVATE
#libjournal
cmark)

enable_testing()
#add_subdirectory(tests)

set(CMAKE_INSTALL_PREFIX
  "C:/tools"
  CACHE PATH "Install path prefix"
  FORCE
)
install(
  TARGETS journal
  DESTINATION "."
)

