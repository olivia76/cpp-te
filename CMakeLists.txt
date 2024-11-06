cmake_minimum_required(VERSION 3.5)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-Wall -Wextra -pedantic -O3)

project(cpp-te VERSION 1.0 LANGUAGES CXX)

add_library(cpp-te INTERFACE)
target_include_directories(cpp-te INTERFACE include)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
