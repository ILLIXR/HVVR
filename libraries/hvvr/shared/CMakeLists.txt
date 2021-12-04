add_library(shared "")

target_sources(shared
        PRIVATE
        array_view.h
        avx.h
        bvh.cpp
        bvh.h
        constants_math.h
        cuda_decl.h
        debug.cpp
        debug.h
        dynamic_array.h
        graphics_types.h
        thread_pool.h
        timer.cpp
        timer.h
        util.h
        util_graphics.h
        vector_math.h
        )

target_include_directories(shared PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
        )