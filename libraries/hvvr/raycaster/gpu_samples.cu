/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "gpu_camera.h"
#include "gpu_samples.h"


namespace hvvr {

CameraBeams::CameraBeams(const GPUCamera& camera) {
    directionalBeams = camera.d_batchSpaceBeams;
    frameJitter = camera.frameJitter;
    lens = camera.lens;
}

CUDA_DEVICE void GetSampleUVsDoF(uint32_t MSAARate,
                                 uint32_t BlockSize,
                                 const vector2* CUDA_RESTRICT tileSubsampleLensPos,
                                 vector2 frameJitter,
                                 vector2 focalToLensScale,
                                 int subsample,
                                 vector2& lensUV,
                                 vector2& dirUV) {
    // Random position on lens. As lensRadius approaches zero, depth of field rays become equivalent to
    // non-depth of field rays, including AA subsample pattern.
    int lensPosIndex =
        (blockIdx.x % DOF_LENS_POS_LOOKUP_TABLE_TILES) * BlockSize * MSAARate + subsample * BlockSize + threadIdx.x;
    // 1 LDG.64 (coherent, but half-efficiency)
    lensUV = tileSubsampleLensPos[lensPosIndex];

    // compile-time constant
    vector2 aaOffset = getSubsampleUnitOffset(MSAARate, frameJitter, subsample);

    // 2 FMA
    dirUV = aaOffset * focalToLensScale - lensUV;
}

} // namespace hvvr
