//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_EQUATION_SOLVER_H
#define CYGNOIDES_DECISION_EQUATION_SOLVER_H

#include <functional>
#include <iostream>

class EquationSolver
{
public:

    /**
     * Solve an equation with Newton-Raphson method.
     * @param func The lhs of the equation. The rhs should be zero.
     * @param derivative_func The derivative of lhs.
     * @param init_x The initial guess of root.
     * @param eps Terminal error.
     * @return Root of the equation. Nan if equation has no solution.
     */
    static float solve_with_der(const std::function<float(float)> &func,
                                const std::function<float(float)> &derivative_func,
                                float init_x = 0.1, float eps = 1E-6);

    /**
     * Solve an equation with Newton-Raphson method. Generate derivative function using numerical way.
     * @param func The lhs of the equation. The rhs should be zero.
     * @param init_x The initial guess of root.
     * @param eps Terminal error.
     * @param delta The tiny delta for numerical derivative.
     * @return Root of the equation. Nan if equation has no solution.
     */
    static float solve(const std::function<float(float)> &func, float init_x = 0.1,
                       float eps = 1E-6, float delta = 1E-7);
};


#endif //CYGNOIDES_DECISION_EQUATION_SOLVER_H
