//
// Created by catslashbin on 23-2-2.
//

#ifndef CYGNOIDES_DECISION_BASIC_AIMING_H
#define CYGNOIDES_DECISION_BASIC_AIMING_H

#include "typing.h"
#include "config.h"

class BasicAiming {
public:
    /**
     * Initialize BasicAiming decision-maker.
     * @param cfg Config loader.
     */
    explicit BasicAiming(Config &cfg);

    ~BasicAiming();

    /**
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     * @note The DetectArmorsFrame object is passed in by `move`. Do not reuse the detection.
     */
    EulerAngles update(ArmorFrameInput detection);

private:
    class BasicAimingImpl;
    std::unique_ptr<BasicAimingImpl> impl;
};

#endif //CYGNOIDES_DECISION_BASIC_AIMING_H
