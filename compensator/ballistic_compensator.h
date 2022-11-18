//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
#define CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H

#define G 9.780

#include "functional"

class BallisticCompensator
{
    float k;
private:
    std::function<float(float)> get_ballistic_func(float v0, float x, float y);

public:
    explicit BallisticCompensator(float k)
    { this->k = k; };

    float ballistic(float v0, float x, float theta) const;

    float calc_shoot_angle(float v0, float x, float y);
};


#endif //CYGNOIDES_DECISION_BALLISTIC_COMPENSATOR_H
