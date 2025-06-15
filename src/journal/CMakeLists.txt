cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX
  "C:/tools"
  CACHE PATH "Install path prefix"
  FORCE
)

project(journal
  LANGUAGES CXX)

add_executable(journal
  blog.cpp
  blog.h
  book.cpp
  book.h
  config.h
  journal.cpp
  journal.h
  main.cpp
  util.h
  util.cpp
)

install(
  TARGETS journal
  DESTINATION "."
)

