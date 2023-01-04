//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
#define CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H

#define GRAVITY 9.780f

#include "functional"

class BallisticCompensator
{
    float k;
private:
    std::function<float(float)> getBallisticFunc(float v0, float x, float y);

    std::function<float(float)> getBallisticDerFunc(float v0, float x);

    float ballisticDer(float v0, float x, float theta) const;

public:

    /**
     * Ballistic compensate calculator.
     * @param k Wind resistance coefficient
     */
    explicit BallisticCompensator(float k)
    { this->k = k; };

    /**
     * Calculate ballistic.
     * @param v0 Initial velocity of the bullet
     * @param x Horizontal distance to barrel
     * @param theta Pitch of gimbal, in rad
     * @return Vertical distance to gimbal
     */
    float ballistic(float v0, float x, float theta) const;

    /**
     * Find shoot angle to the target.
     * @param v0 Initial velocity of the bullet
     * @param x Horizontal distance to the target
     * @param y Vertical distance to the target
     * @return Pitch of gimbal, in rad
     */
    float calcShootAngle(float v0, float x, float y);
};


#endif //CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
