cmake_minimum_required(VERSION 3.7)

project(journal LANGUAGES C)

if(MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
endif()

add_executable(journal
main.c
../tinytemplate/src/tinytemplate.c
../tinytemplate/src/tinytemplate.h
)

target_include_directories(journal PRIVATE ../tinytemplate/src)

target_link_libraries(journal PRIVATE cmark)

set(CMAKE_INSTALL_PREFIX "/tools" CACHE PATH "Install path prefix" FORCE)

install(TARGETS journal DESTINATION ".")
