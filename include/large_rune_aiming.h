//
// Created by catslashbin on 23-2-18.
//

#ifndef CYGNOIDES_DECISION_LARGE_RUNE_H
#define CYGNOIDES_DECISION_LARGE_RUNE_H

#include "typing.h"
#include "config.h"

class LargeRuneAiming {
public:
    /**
     * Initialize BasicAiming decision-maker.
     * @param cfg Config loader.
     */
    explicit LargeRuneAiming(Config &cfg);

    ~LargeRuneAiming();

    /**
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     * @note The DetectArmorsFrame object is passed in by `move`. Do not reuse the detection.
     */
    EulerAngles update(FrameInput detection);

private:
    class LargeRuneAimingImpl;
    std::unique_ptr<LargeRuneAimingImpl> impl;
};

#endif //CYGNOIDES_DECISION_LARGE_RUNE_H
