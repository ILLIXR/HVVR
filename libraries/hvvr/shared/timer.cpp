/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "timer.h"
#include <chrono>

namespace hvvr {

Timer::Timer() {
    startTime = getCurrentMicroseconds();
    lastTime = startTime;
}

double Timer::get() {
    uint64_t currentTime = getCurrentMicroseconds();
    lastTime = currentTime;

    return (double) (currentTime - lastTime);
}

double Timer::getElapsed() {
    uint64_t currentTime = getCurrentMicroseconds();

    return (double ) (currentTime - startTime);
}
uint64_t Timer::getCurrentMicroseconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::
                                                                     now().time_since_epoch()).count();
}

} // namespace hvvr
