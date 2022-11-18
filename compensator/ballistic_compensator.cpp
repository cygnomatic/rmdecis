//
// Created by catslashbin on 22-11-18.
//

#include "ballistic_compensator.h"
#include "../utils/equation_solver.h"
#include <cmath>

std::function<float(float)> BallisticCompensator::get_ballistic_func(float v0, float x, float y)
{
    return [v0, x, y, this](float theta){return ballistic(v0, x, theta) - y;};
}

float BallisticCompensator::ballistic(float v0, float x, float theta) const
{
    return G / (k * k) * logf(1 - (k * x) / (v0 * cosf(theta))) +
            x * (G / (k * v0 * cosf(theta)) + tanf(theta));
}

float BallisticCompensator::calc_shoot_angle(float v0, float x, float y)
{
    return EquationSolver::solve(get_ballistic_func(v0, x, y));
}
