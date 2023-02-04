//
// Created by catslashbin on 23-2-1.
//

#ifndef CYGNOIDES_DECISION_TRIG_FITTING_H
#define CYGNOIDES_DECISION_TRIG_FITTING_H

#include <kiss_fftr.h>
#include "rmdecis/core.h"

/**
 * A C++ wrapper for kissfft. TrigFitting (real input) only.
 */
 
#define MAX_FFT_SAMPLE_NUM 5000

class TrigFitting
{
    std::vector<float> data_; // Data, y list.
    kiss_fft_cpx fftr_result_[MAX_FFT_SAMPLE_NUM] = {}; // Result buffer

    Time last_update_time_;
    float avg_update_interval_; // Avg time between an update and the next.

public:

    std::function<float(float)> fit_func;
    float amp{}, omega{}, phase{}, offset{};

    explicit TrigFitting(Config& cfg);

    void update(Time time, float y);

    void fftPreEstimate();
};


#endif //CYGNOIDES_DECISION_TRIG_FITTING_H
