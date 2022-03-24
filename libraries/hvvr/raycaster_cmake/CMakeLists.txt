#add_definitions(-w)
add_library(raycaster "")

target_link_libraries(raycaster OpenGL::GL)

message(STATUS "OpenGL Include Dir: ${OPENGL_INCLUDE_DIR}")

set_property(TARGET raycaster PROPERTY CUDA_ARCHITECTURES 62)

target_include_directories(raycaster PUBLIC
        "../raycaster"
        "../shared"
        PRIVATE
        "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
        "${OPENGL_INCLUDE_DIR}"
        )

target_sources(raycaster
        PRIVATE
        ../raycaster/linux_support.h
        ../raycaster/camera.cu
        ../raycaster/foveated.cpp
        ../raycaster/frusta.cu
        ../raycaster/gpu_camera.cu
        ../raycaster/gpu_context.cu
        ../raycaster/gpu_foveated.cu
        ../raycaster/gpu_image.cu
        ../raycaster/gpu_samples.cu
        ../raycaster/gpu_scene_state.cu
        ../raycaster/intersect.cu
        ../raycaster/light.cpp
        ../raycaster/model.cpp
        ../raycaster/raycaster.cpp
        ../raycaster/remap.cu
        ../raycaster/render.cpp
        ../raycaster/resolve.cu
        ../raycaster/sample_hierarchy.cpp
        ../raycaster/samples.cpp
        ../raycaster/scene.cpp
        ../raycaster/scene_update.cpp
        ../raycaster/shading_helpers.h
        ../raycaster/texture.cu
        ../raycaster/traversal.cpp
        ../raycaster/traverse_avx.cpp
        ../raycaster/traverse_ref_bvh.cpp
        ../raycaster/traverse_ref_frustum.cpp
        ../shared/array_view.h
        ../shared/avx.h
        ../shared/bvh.cpp
        ../shared/bvh.h
        ../shared/constants_math.h
        ../shared/cuda_decl.h
        ../shared/debug.cpp
        ../shared/debug.h
        ../shared/dynamic_array.h
        ../shared/graphics_types.h
        ../shared/thread_pool.h
        ../shared/timer.cpp
        ../shared/timer.h
        ../shared/util.h
        ../shared/util_graphics.h
        ../shared/vector_math.h
        )