#pragma once

/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "util.h"

#include <stdint.h>

struct Children {
    char PADDING[4];
    uint32_t offset[4];
};

struct Leaf {
    uint32_t triIndex[5];
};

// A bounding volume hierarchy node, optimized for traversal.
struct ALIGN(32) BVHNode {
    float xMax[4], xNegMin[4];
    float yMax[4], yNegMin[4];
    float zMax[4], zNegMin[4];

    // A struct representing a 32 byte section of a BVHNode that contains the connectivity information.
    struct ALIGN(32) BoxData {
        unsigned leafMask;
        union {
            Children children;
            Leaf leaf;
        };
        char PADDING2[8];
    } boxData;
};
static_assert(sizeof(BVHNode) == 128, "BVHNode size changed, was this intentional?");
