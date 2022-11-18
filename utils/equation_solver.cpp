//
// Created by catslashbin on 22-11-18.
//

#include <cmath>
#include "equation_solver.h"

float EquationSolver::solve_with_der(const std::function<float(float)> &func,
                                     const std::function<float(float)> &derivative_func,
                                     float init_x, float eps)
{
    float x = init_x;
    while (fabs(func(x)) > eps)
    {
        x = x - func(x) / derivative_func(x);
    }
    return x;
}

float EquationSolver::solve(const std::function<float(float)> &func, float init_x, float eps, float delta)
{
    return solve_with_der(func,
                          [&func, delta](float x) { return (func(x + delta) - func(x)) / delta; },
                          init_x, eps);
}

