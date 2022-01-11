add_executable(modelconvert "")

target_sources(modelconvert PRIVATE modelconvert.cpp)

target_link_libraries(modelconvert samples_shared)