cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(HVVR CXX CUDA)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(libraries/hvvr)
add_subdirectory(projects)