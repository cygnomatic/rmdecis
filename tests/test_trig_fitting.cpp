//
// Created by catslashbin on 23-2-6.
//

#include "predictor/trig_fitting.h"
#include <fenv.h>
#include <random>

#define AMP 1.12
#define OMEGA 0.83
#define PHASE 3.2
#define OFFSET 4.2

#define SAMPLE_FREQ 50
#define SAMPLE_RANGE 100
#define SAMPLE_NUM (SAMPLE_FREQ * SAMPLE_RANGE)

int main()
{
    feenableexcept(FE_INVALID | FE_OVERFLOW);

    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<float> dist(0, 1);

    Config cfg("../../config/config.yml");
    TrigFitting trig_fitting(cfg);
    auto init_time = Time(0, 0);
    auto time = Time(0, 0);

    for (int i = 0; i < SAMPLE_NUM; ++i)
    {
        trig_fitting.update(time, AMP * std::sin(OMEGA * (time - init_time) + PHASE) + OFFSET + dist(e2));
        time = time + (1.f / SAMPLE_FREQ);

        // if (i % 10 == 0)
            info("amp {}, omega {}, phase {}, offset {}",
                 trig_fitting.amp_, trig_fitting.omega_, trig_fitting.phase_, trig_fitting.offset_);
    }

}