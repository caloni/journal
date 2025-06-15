cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(journal
  LANGUAGES CXX)

add_library(libjournal
  blog.cpp
  blog.h
  book.cpp
  book.h
  config.cpp
  config.h
  iblog.h
  ibook.h
  iconfig.h
  ijournal.h
  ishell.h
  journal.cpp
  journal.h
  shell.cpp
  shell.h
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

