cmake_minimum_required(VERSION 3.7)

project(journal LANGUAGES C)

add_library(libjournal journal.c journal.h)

add_executable(journal main.c)

target_link_libraries(journal PRIVATE libjournal cmark)

add_subdirectory(test)

set(CMAKE_INSTALL_PREFIX "C:/tools" CACHE PATH "Install path prefix" FORCE)

install(TARGETS journal DESTINATION ".")

