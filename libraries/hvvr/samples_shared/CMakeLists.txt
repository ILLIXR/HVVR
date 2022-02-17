add_library(samples_shared "")

target_include_directories(samples_shared PUBLIC
        ./
        3rdparty/
        PRIVATE
        "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
        )
target_sources(samples_shared PRIVATE
        model_import.cpp
        model_import_bin.cpp
        model_import_fbx.cpp
        )

target_link_libraries(samples_shared raycaster)