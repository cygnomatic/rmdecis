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
    kiss_fft_cpx fftr_result[MAX_FFT_SAMPLE_NUM]{};
    float amp{}, omega{}, phase{}, offset{};
    std::function<float(float)> fit_func;

public:
    explicit TrigFitting(Config& cfg);

    ~TrigFitting();

    float Update(Time time, float y);

    void fftr(const std::vector<float> &data);
};


#endif //CYGNOIDES_DECISION_TRIG_FITTING_H
