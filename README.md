# HVVR on Linux

## Building with Docker
1. Configure docker to support GPU passthrough by installing components specified [here](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html) in NVIDIA's docs.
2. Use the official CUDA image on Docker Hub `nvidia/cuda` and select a `devel` version to use in tags. `11.4.2-devel-ubuntu20.04` is the latest compatible version.
3. Specify nvcc path using `CMAKE_CUDA_COMPILER`.

## Building with Native Environment
1. Configure environment
   1. cudatoolkit-dev (**regular cuda runtime without dev DOES NOT contain nvcc**) => 11
   2. compatible host compiler. To compile with CUDA 11 and clang, clang major version has to be <= 12. For gcc, please see the table below.
   3. cmake (latest)

| CUDA version  | gcc version |
| ------------- | ----------- |
| 11.4.1 ~ 11.5 | 11          |
| 11.1 ~ 11.4.0 | 10          |

2. Configure CUDA environment for CMake

   Currently, CMake's automatic  detection for CUDA has multiple issues. Theoretically, a **native** installation of all required libraries should not need any configuration. However, with a non-standard CUDA installation or in case of auto detection failure, CUDA paths need to be manually specified.

```
-DCUDAToolkit_ROOT="PATH TO CUDA ROOT FOLDER"
-DCUDA_TOOLKIT_ROOT_DIR="PATH TO CUDA ROOT FOLDER"
-DCMAKE_CUDA_COMPILER="PATH TO NVCC (located in bin/ of cuda root)"
-DCMAKE_CUDA_HOST_COMPILER="PATH TO COMPATIBLE GCC"
-DCMAKE_CUDA_FLAGS="-ccbin PATH TO COMPATIBLE GCC"
```

   Please note that failing to set any of these flags will result in a generic error message by CMake. Go into the build directory and view error logs for further investigation.
