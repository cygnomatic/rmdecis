//
// Created by catslashbin on 22-11-18.
//

#include "ballistic_compensator.h"
#include "../utils/equation_solver.h"
#include <cmath>

std::function<float(float)> BallisticCompensator::get_ballistic_func(float v0, float x, float y)
{
    return [v0, x, y, this](float theta) { return ballistic(v0, x, theta) - y; };
}

std::function<float(float)> BallisticCompensator::get_ballistic_der_func(float v0, float x)
{
    return [v0, x, this](float theta) { return ballistic_der(v0, x, theta); };
}

float BallisticCompensator::ballistic_der(float v0, float x, float theta) const
{
    float Sec = 1 / cosf(theta);
    float Tan = tanf(theta);
    float ret = -(G * x * Tan * Sec) / (k * v0 * (1 - (k * x * Sec) / v0)) +
                x * (Sec * Sec + (G * Sec * Tan) / (k * v0));
    return ret;
}

float BallisticCompensator::ballistic(float v0, float x, float theta) const
{
    return G / (k * k) * logf(1 - (k * x) / (v0 * cosf(theta))) +
           x * (G / (k * v0 * cosf(theta)) + tanf(theta));
}

float BallisticCompensator::calc_shoot_angle(float v0, float x, float y)
{
    return EquationSolver::solve_with_der(get_ballistic_func(v0, x, y),
                                          get_ballistic_der_func(v0, x));
}
