//
// Created by catslashbin on 22-11-18.
//

#include "ballistic_compensator.h"
#include "ceres/ceres.h"

struct ProjectileCostFunctor {
    ProjectileCostFunctor(double x, double y, double v0) : x_(x), y_(y), v0_(v0) {}

    template<typename T>
    bool operator()(const T *const theta, T *residual) const {
        const T tan_theta = ceres::tan(*theta);
        const T v0 = T(v0_);
        const T cos_theta = ceres::cos(*theta);
        const T x = T(x_);
        const T y = T(y_);
        const T g = T(GRAVITY); // gravitational acceleration

        residual[0] = tan_theta * x - g / (T(2.0) * v0 * v0 * cos_theta * cos_theta) * x * x - y;
        return true;
    }

private:
    const double x_;
    const double y_;
    const double v0_;
};

double BallisticCompensator::calcShootAngle(double x, double y, double v0, double init_theta) const {

    double theta = init_theta;

    ceres::Problem problem;
    problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<ProjectileCostFunctor, 1, 1>(new ProjectileCostFunctor(x, y, v0)),
            nullptr, &theta
    );

    ceres::Solver::Options options;
    options.max_num_iterations = max_num_iterations;
    options.linear_solver_type = ceres::DENSE_QR;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    if (summary.IsSolutionUsable()) {
        // According to REP105, theta decrease as the gimbal roll up.
        return -1.0 * theta;
    } else {
        return NAN;
    }

}

BallisticCompensator::BallisticCompensator(Config &cfg) :
        max_num_iterations(cfg.get<int>("aiming.compensator.maxNumIterations", 100)) {}
