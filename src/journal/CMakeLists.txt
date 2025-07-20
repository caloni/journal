cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(journal LANGUAGES C)

if(MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
endif()

add_executable(journal main.c)

target_link_libraries(journal PRIVATE cmark)

set(CMAKE_INSTALL_PREFIX "/tools" CACHE PATH "Install path prefix" FORCE)

install(TARGETS journal DESTINATION ".")
