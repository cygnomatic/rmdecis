//
// Created by catslashbin on 23-2-1.
//

#ifndef CYGNOIDES_DECISION_TRIG_FITTING_H
#define CYGNOIDES_DECISION_TRIG_FITTING_H

#include <kissfft/kiss_fftr.h>
#include <ceres/ceres.h>

#include "rmdecis/core.h"

/**
 * A C++ wrapper for kissfft. TrigFitting (real input) only.
 */
 
#define MAX_FFT_SAMPLE_NUM 5000 // Must be even
#define MIN_FFT_SAMPLE_NUM 200

#define AVG_AMP 1.0
#define AVG_OMEGA 0.9

static_assert(MAX_FFT_SAMPLE_NUM > MIN_FFT_SAMPLE_NUM);
static_assert(MAX_FFT_SAMPLE_NUM % 2 == 0);

class TrigFitting
{
    std::vector<float> sample_y; // Data, y list.
    std::vector<float> sample_t;
    int sample_num_ = 0;

    kiss_fft_cpx fftr_result_[MAX_FFT_SAMPLE_NUM] = {}; // Result buffer

    Time init_time_;
    Time last_update_time_;
    float avg_update_interval_; // Avg time between an update and the next.

    float k_omega_range, k_amp_range;

    ceres::Solver::Options ceres_options_;

public:

    float amp_ = 1.f, omega_ = 1.f, phase_ = 0.f, offset_ = 0.f;

    explicit TrigFitting(Config& cfg);

    void update(Time time, float y);

    void fftPreEstimate();

    float predict(Time time);

    void nlsFitting();

    bool isValid();
};


#endif //CYGNOIDES_DECISION_TRIG_FITTING_H
