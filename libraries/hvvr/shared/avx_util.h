//
// Created by steven on 2/25/22.
//

#ifndef HVVR_AVX_UTIL_H
#define HVVR_AVX_UTIL_H

#include <immintrin.h>
#include <stdint.h>

inline uint32_t clearLowestBit(uint32_t a) { return _blsr_u32(a); }
inline uint64_t clearLowestBit(uint64_t a) { return _blsr_u64(a); }

#endif // HVVR_AVX_UTIL_H
