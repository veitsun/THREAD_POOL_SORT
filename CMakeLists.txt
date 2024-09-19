cmake_minimum_required(VERSION 3.20.1)
project(general_matrix_multiply)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CUDA_ARCHITECTURES 60)
include_directories(include)
add_subdirectory(src)