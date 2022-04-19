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
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-mf16c>)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3 REQUIRED)

add_subdirectory(libraries/hvvr)
add_subdirectory(projects)

if (EXISTS "/opt/fbxsdk")
    target_compile_definitions(samples_shared PRIVATE
            MODEL_IMPORT_ENABLE_FBX=1)
    target_compile_definitions(modelconvert PRIVATE
            MODEL_IMPORT_ENABLE_FBX=1)
    add_library( fbxsdk SHARED IMPORTED )
    set_target_properties( fbxsdk PROPERTIES IMPORTED_LOCATION "/opt/fbxsdk/lib/gcc/x64/release/libfbxsdk.so" )
    find_package(LibXml2)
    target_link_libraries(samples_shared fbxsdk)
    target_link_libraries(samples_shared ${LIBXML2_LIBRARY})
    target_include_directories(samples_shared PUBLIC "/opt/fbxsdk/include")
endif ()