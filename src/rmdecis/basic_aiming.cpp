//
// Created by catslashbin on 23-2-2.
//

#include "basic_aiming.h"
#include "rmdecis_impl/basic_aiming_impl.h"

BasicAiming::BasicAiming(Config &config_loader) : impl(new BasicAimingImpl(config_loader)) {}

BasicAiming::~BasicAiming() = default;

EulerAngles BasicAiming::update(ArmorFrameInput detection, cv::Mat *debug_img) {
    return impl->update(std::move(detection), debug_img);
}
