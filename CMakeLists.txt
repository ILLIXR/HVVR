cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(HVVR CXX CUDA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3 REQUIRED)
find_package(CUDA REQUIRED)

include_directories("${CUDA_INCLUDE_DIRS}")

add_subdirectory(libraries/hvvr)
add_subdirectory(projects)
