//
// Created by catslashbin on 23-2-18.
//

#include "large_rune_aiming.h.t"
#include "rmdecis_impl/large_rune_aiming_impl.h.t"

LargeRuneAiming::LargeRuneAiming(Config &config) : impl(new LargeRuneAimingImpl(config)) {}

LargeRuneAiming::~LargeRuneAiming() = default;

EulerAngles LargeRuneAiming::update(FrameInput detection) {
    return impl->update(std::move(detection));
}
