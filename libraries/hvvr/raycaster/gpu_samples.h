#pragma once

/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "constants_math.h"
#include "cuda_decl.h"
#include "kernel_constants.h"
#include "samples.h"
#include "vector_math.h"


namespace hvvr {

class GPUCamera;

CUDA_HOST_DEVICE_INL vector2 tapLocation(uint32_t MSAARate, int subsampleIndex, float spinAngle, float startOffset, float& radius) {
    // From http://graphics.cs.williams.edu/papers/DeepGBuffer16/Mara2016DeepGBuffer.pdf
    constexpr int tau[] = {
        //  0   1   2   3   4   5   6   7   8   9
        1,  1,  1,  2,  3,  2,  5,  2,  3,  2,   // 0
        3,  3,  5,  5,  3,  4,  7,  5,  5,  7,   // 1
        9,  8,  5,  5,  7,  7,  7,  8,  5,  8,   // 2
        11, 12, 7,  10, 13, 8,  11, 8,  7,  14,  // 3
        11, 11, 13, 12, 13, 19, 17, 13, 11, 18,  // 4
        19, 11, 11, 14, 17, 21, 15, 16, 17, 18,  // 5
        13, 17, 11, 17, 19, 18, 25, 18, 19, 19,  // 6
        29, 21, 19, 27, 31, 29, 21, 18, 17, 29,  // 7
        31, 31, 23, 18, 25, 26, 25, 23, 19, 34,  // 8
        19, 27, 21, 25, 39, 29, 17, 21, 27, 29}; // 9

    const int numSpiralTurns = tau[MSAARate];
    // Radius relative to ssR
    float alpha = float(subsampleIndex + startOffset) * (1.0f / MSAARate);
    float angle = alpha * (numSpiralTurns * 6.28f) + spinAngle;

    radius = sqrtf(alpha);
    return vector2(cosf(angle), sinf(angle));
}

CUDA_HOST_DEVICE_INL vector2 getSubsampleUnitOffset(uint32_t MSAARate,
                                                    vector2 sampleJitter,
                                                    int subsampleIndex,
                                                    float extraSpinAngle = 0.0f) {
    (void)sampleJitter;
    float spinAngle = extraSpinAngle;
#if JITTER_SAMPLES
    spinAngle += sampleJitter.x * 6.28f;
#endif
    float startOffset = 0.5f;

    float radius;
    vector2 unitDiskLoc = tapLocation(MSAARate, subsampleIndex, spinAngle, startOffset, radius);

    return vector2(unitDiskLoc.x * radius, unitDiskLoc.y * radius);
}

CHDI DirectionalBeam operator*(matrix3x3 M, DirectionalBeam beam) {
    DirectionalBeam mBeam;
    mBeam.centerRay = M * beam.centerRay;
    mBeam.du = M * beam.du;
    mBeam.dv = M * beam.dv;
    return mBeam;
}

struct CameraBeams {
    DirectionalBeam* directionalBeams;
    vector2 frameJitter;
    ThinLens lens;
    CameraBeams(const GPUCamera& camera);
};

CUDA_DEVICE_INL DirectionalBeam GetDirectionalSample3D(uint32_t sampleIndex,
                                                       CameraBeams cameraBeams,
                                                       matrix4x4 cameraToWorld) {
    return matrix3x3(cameraToWorld) * cameraBeams.directionalBeams[sampleIndex];
}

CUDA_DEVICE void GetSampleUVsDoF(uint32_t MSAARate,
                                 uint32_t BlockSize,
                                 const vector2* CUDA_RESTRICT tileSubsampleLensPos,
                                 vector2 frameJitter,
                                 vector2 focalToLensScale,
                                 int subsample,
                                 vector2& lensUV,
                                 vector2& dirUV);

} // namespace hvvr
