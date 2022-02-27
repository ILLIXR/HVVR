add_executable(modelviewer "" ModelViewerConfig.h)

target_link_libraries(raycaster glfw)
target_link_libraries(raycaster GLEW::GLEW)

set_property(TARGET modelviewer PROPERTY CUDA_ARCHITECTURES 62)

target_include_directories(modelviewer PRIVATE
        "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
        "${OPENGL_INCLUDE_DIR}"
        )

target_sources(modelviewer PRIVATE ModelViewerLinux.cpp)

set_target_properties(modelviewer PROPERTIES
        CUDA_RESOLVE_DEVICE_SYMBOLS ON)
target_link_libraries(modelviewer PRIVATE samples_shared)
target_link_libraries(modelviewer PRIVATE raycaster)

if(EXISTS "sponza.bin")
FILE(COPY sponza.bin DESTINATION "${CMAKE_BINARY_DIR}/projects/hvvr_samples/modelviewer/scenes")
endif()