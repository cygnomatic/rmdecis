//
// Created by catslashbin on 23-2-1.
//

#include "trig_fitting.h"

TrigFitting::TrigFitting(Config& cfg) {

}


float TrigFitting::Update(Time time, float y) {

}

void TrigFitting::fftr(const std::vector<float>& data) {
    auto fftr_cfg = kiss_fftr_alloc(data.size(), false, nullptr, nullptr);
    kiss_fftr(fftr_cfg, data.data(), fftr_result);
}