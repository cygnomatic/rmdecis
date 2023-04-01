//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
#define CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H

#define GRAVITY 9780.f

#include <functional>
#include <cmath>
#include "rmdecis/core.h"

#include "utils/equation_solver.h"

class BallisticCompensator {

private:
    int max_num_iterations;

public:

    /**
     * Ballistic compensate calculator.
     * @param k Wind resistance coefficient
     */
    explicit BallisticCompensator(Config &cfg);

    /**
     * Find shoot angle to the target.
     * @param v0 Initial velocity of the bullet, mm/s
     * @param x Horizontal distance to the target, mm
     * @param y Vertical distance to the target, in mm
     * @return Pitch of gimbal, in rad
     */
    double calcShootAngle(double x, double y, double v0, double init_theta = 0.0) const;

};


#endif //CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
