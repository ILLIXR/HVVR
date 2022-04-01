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

if (EXISTS "/opt/fbxsdk")
    target_compile_definitions(samples_shared PRIVATE
            MODEL_IMPORT_ENABLE_FBX=1)
    add_library( fbxsdk SHARED IMPORTED )
    set_target_properties( fbxsdk PROPERTIES IMPORTED_LOCATION "/opt/fbxsdk/lib/gcc/x64/release/libfbxsdk.so" )
    find_package(LibXml2)
    target_link_libraries(samples_shared fbxsdk)
    target_link_libraries(samples_shared ${LIBXML2_LIBRARY})
    target_include_directories(samples_shared PUBLIC "/opt/fbxsdk/include")
endif ()