//
// Created by catslashbin on 23-2-2.
//

#include "basic_aiming.h"
#include "rmdecis_impl/basic_aiming_impl.h"

BasicAiming::BasicAiming(ConfigLoader &config_loader) : impl(new BasicAimingImpl(config_loader)) {}

EulerAngles BasicAiming::update(DetectArmorsFrame &detection) {
    return impl->update(detection);
}