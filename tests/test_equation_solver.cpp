//
// Created by catslashbin on 22-11-18.
//

#include <cmath>
#include "../src/utils/equation_solver.h"

inline float func(float x) {return x - x*x + 0.5;}

int main()
{
    std::cout << EquationSolver::solve(func);
    return 0;
}