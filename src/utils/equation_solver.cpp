//
// Created by catslashbin on 22-11-18.
//

#include "equation_solver.h"

#define MAX_N_ITERATION 100

float EquationSolver::solveWithDer(const std::function<float(float)> &func,
                                   const std::function<float(float)> &derivative_func,
                                   float init_x, float eps) {
    float x = init_x;
    int n_iter = 0;
    while (n_iter < MAX_N_ITERATION) // To prevent infinite loop.
    {
        x = x - func(x) / derivative_func(x);
        if (fabs(func(x)) < eps)
            return x;
        n_iter++;
    }
    return nanf("");
}

float EquationSolver::solve(const std::function<float(float)> &func, float init_x, float eps, float delta) {
    return solveWithDer(func,
                        [&func, delta](float x) { return (func(x + delta) - func(x)) / delta; },
                        init_x, eps);
}

