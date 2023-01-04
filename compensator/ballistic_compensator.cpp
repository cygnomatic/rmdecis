//
// Created by catslashbin on 22-11-18.
//

#include "ballistic_compensator.h"
#include "../utils/equation_solver.h"
#include <cmath>

std::function<float(float)> BallisticCompensator::getBallisticFunc(float v0, float x, float y)
{
    return [v0, x, y, this](float theta) { return ballistic(v0, x, theta) - y; };
}

std::function<float(float)> BallisticCompensator::getBallisticDerFunc(float v0, float x)
{
    return [v0, x, this](float theta) { return ballisticDer(v0, x, theta); };
}

float BallisticCompensator::ballisticDer(float v0, float x, float theta) const
{
    float Sec = 1 / cosf(theta);
    float Tan = tanf(theta);
    float ret = -(GRAVITY * x * Tan * Sec) / (k * v0 * (1 - (k * x * Sec) / v0)) +
                x * (Sec * Sec + (GRAVITY * Sec * Tan) / (k * v0));
    return ret;
}

float BallisticCompensator::ballistic(float v0, float x, float theta) const
{
    return GRAVITY / (k * k) * logf(1 - (k * x) / (v0 * cosf(theta))) +
           x * (GRAVITY / (k * v0 * cosf(theta)) + tanf(theta));
}

float BallisticCompensator::calcShootAngle(float v0, float x, float y)
{
    return EquationSolver::solveWithDer(getBallisticFunc(v0, x, y),
                                        getBallisticDerFunc(v0, x));
}
