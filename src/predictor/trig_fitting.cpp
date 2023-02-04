//
// Created by catslashbin on 23-2-1.
//

#include "trig_fitting.h"

TrigFitting::TrigFitting(Config &cfg) {
    avg_update_interval_ = cfg.get<float>("predictor.rune.avgUpdateTimeInterval");
}


void TrigFitting::update(Time time, float y) {
    // Calculate avg. update time
    avg_update_interval_ += (1.f / float(data_.size())) * (time - last_update_time_);
    last_update_time_ = time;

    // Pre-estimate data with FFT
    // TODO: We do not need to estimate with FFT every update. Setup a threshold.
    //      When the difference go bigger than the threshold than run FFT.
    fftPreEstimate();
}

void TrigFitting::fftPreEstimate() {

    // Perform FFT
    auto fftr_cfg = kiss_fftr_alloc(data_.size(), false, nullptr, nullptr);
    kiss_fftr(fftr_cfg, data_.data(), fftr_result_);

    // Calculate Max Freq
    int max_idx = 1; // NOTE: Start from the second freq [1], as [0] is a high peak related to the offset.
    float max_norm_sqr = 0, norm_sqr;
    for (int i = 1; i < data_.size(); ++i) {
        norm_sqr = fftr_result_[i].i * fftr_result_[i].i + fftr_result_[i].r * fftr_result_[i].r;
        if (norm_sqr > max_norm_sqr) {
            max_idx = i;
            max_norm_sqr = norm_sqr;
        }
    }


    // Retrieve estimation from FFT result
    float mean = std::reduce(data_.begin(), data_.end()) / float(data_.size());
    float deviation = std::inner_product(data_.begin(), data_.end(), data_.begin(), 0.f)
                      / float(data_.size()) - offset * offset;
    float freq = float (max_idx) / (float(data_.size()) * avg_update_interval_);

    offset = mean;
    phase = std::sqrt(deviation) * std::sqrt(2.f);

}