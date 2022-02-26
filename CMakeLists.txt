cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(HVVR CXX CUDA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

#add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-v>)
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:--verbose>)
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-O0>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-mavx>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-mavx2>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-msse>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-msse2>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-mfma>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-mbmi>)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3 REQUIRED)

add_subdirectory(libraries/hvvr)
add_subdirectory(projects)
