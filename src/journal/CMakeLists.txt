cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(journal
  LANGUAGES CXX)

add_library(libjournal
  application.cpp
  application.h
  blog_generator.cpp
  book_generator.cpp
  composite_generator.cpp
  config.cpp
  intercepting_parser.cpp
  journal.h
  markdown_parser.cpp
  metadata_interceptor.cpp
  shell.cpp
  README.md
)

add_executable(journal
  main.cpp
)
target_link_libraries(journal PRIVATE libjournal)

enable_testing()
add_subdirectory(tests)

set(CMAKE_INSTALL_PREFIX
  "C:/tools"
  CACHE PATH "Install path prefix"
  FORCE
)
install(
  TARGETS journal
  DESTINATION "."
)

