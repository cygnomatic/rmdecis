//
// Created by catslashbin on 23-2-18.
//

#include "large_rune_aiming.h"
#include "rmdecis_impl/large_rune_aiming_impl.h"

LargeRuneAiming::LargeRuneAiming(Config &config) : impl(new LargeRuneAimingImpl(config)) {}

LargeRuneAiming::~LargeRuneAiming() = default;

EulerAngles LargeRuneAiming::update(RuneFrameInput detection, cv::Mat *debug_frame) {
    return impl->update(std::move(detection), debug_frame);
}
