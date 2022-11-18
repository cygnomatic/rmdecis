//
// Created by catslashbin on 22-11-18.
//

#include <cmath>
#include "../utils/equation_solver.h"

inline float func(float x) {return 1;}

int main()
{
    std::cout << EquationSolver::solve(func);
    return 0;
}