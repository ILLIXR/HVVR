add_executable(modelconvert "")

set_property(TARGET modelconvert PROPERTY CUDA_ARCHITECTURES 62)
target_include_directories(modelconvert PRIVATE
        "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
        )

target_sources(modelconvert PRIVATE modelconvert.cpp)

target_link_libraries(modelconvert samples_shared)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/models/)
    FILE(COPY models/ DESTINATION "${CMAKE_BINARY_DIR}/projects/hvvr_samples/modelconvert/models")
endif()