add_library(raycaster "")
target_sources(raycaster
        PRIVATE
        camera.cu
        foveated.cpp
        frusta.cu
        gpu_camera.cu
        gpu_context.cu
        gpu_foveated.cu
        gpu_image.cu
        gpu_samples.cu
        gpu_scene_state.cu
        intersect.cu
        light.cpp
        model.cpp
        raycaster.cpp
        remap.cu
        render.cpp
        resolve.cu
        sample_hierarchy.cpp
        samples.cpp
        scene.cpp
        scene_update.cpp
        shading_helpers.h
        texture.cu
        traversal.cpp
        traverse_avx.cpp
        traverse_ref_bvh.cpp
        traverse_ref_frustum.cpp
        )

target_include_directories(raycaster PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "../shared/"
        )

target_link_libraries(raycaster PUBLIC shared)