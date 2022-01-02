cmake_minimum_required(VERSION 3.20)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(HVVR CXX CUDA)

set(CMAKE_CXX_STANDARD 14)
string(APPEND CMAKE_CUDA_FLAGS " -dc")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(libraries/hvvr)