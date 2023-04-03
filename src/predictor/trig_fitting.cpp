//
// Created by catslashbin on 23-2-1.
//

#include "trig_fitting.h"

using namespace ceres;

struct TrigResidual
{
    TrigResidual(double x, double y)
            : x_(x), y_(y) {}

    template<typename T>
    bool operator()(const T *const amp, const T *const omega, const T *const phase, const T *const offset,
                    T *residual) const
    {
        residual[0] = y_ - (amp[0] * ceres::sin(omega[0] * x_ + phase[0]) + offset[0]);
        return true;
    }

private:
    const double x_;
    const double y_;
};

TrigFitting::TrigFitting(Config &cfg)
{
    avg_update_interval_ = cfg.get<float>("predictor.rune.avgUpdateTimeInterval", 0.02);
    k_amp_range = cfg.get<float>("predictor.rune.predAmpRange", 2.0);
    k_omega_range = cfg.get<float>("predictor.rune.predOmegaRange", 2.0);

    ceres_options_.max_num_iterations = 25;
    ceres_options_.linear_solver_type = ceres::DENSE_QR;
    ceres_options_.logging_type = ceres::SILENT;
    // ceres_options_.minimizer_progress_to_stdout = true;
}

void TrigFitting::update(Time time, float y)
{

    sample_t.push_back(time - init_time_);
    sample_y.push_back(y);
    sample_num_++;

    // Calculate avg. update time
    if (init_time_.isDefined)
    {
        avg_update_interval_ +=
                ((time - last_update_time_) - avg_update_interval_) * (1.f / (float(sample_num_) + 1.f));
    } else
    {
        init_time_ = time;
    }
    last_update_time_ = time;

    // Ensure the length out the data
    if (sample_num_ > MAX_FFT_SAMPLE_NUM) // Pop out first sample
    {
        sample_num_--;
        sample_t.erase(sample_t.begin());
        sample_y.erase(sample_y.begin());
    }
    if (sample_num_ < MIN_FFT_SAMPLE_NUM || sample_num_ % 2 != 0) // Not enough samples or odd sample_num
        return;

    // Pre-estimate data with FFT
    // TODO: We do not need to estimate with FFT every update. Setup a threshold.
    //      When the difference go bigger than the threshold than run FFT.
    if (!isValid() || sample_num_ == MIN_FFT_SAMPLE_NUM)
    {
        fftPreEstimate();
    }

    // Perform NLS Fitting
    nlsFitting();
}

inline bool TrigFitting::isValid()
{
    if ((std::abs(amp_) - AVG_AMP) > k_amp_range
        || (std::abs(omega_) - AVG_OMEGA) > k_omega_range) {
        warn("Bad Prediction. Rerun FFT estimation.");
        return false;
    }
    return true;
}

float TrigFitting::predict(Time time)
{
    return amp_ * std::sin(omega_ * (time - init_time_) + phase_) + offset_;
}

void TrigFitting::fftPreEstimate()
{

    // Perform FFT
    auto fftr_cfg = kiss_fftr_alloc(sample_num_, false, nullptr, nullptr);
    kiss_fftr(fftr_cfg, sample_y.data(), fftr_result_);

    // Calculate Max Freq
    int max_idx = 1; // NOTE: Start from the second freq [1], as [0] is a high peak related to the offset.
    float max_norm_sqr = 0, norm_sqr;
    for (int i = 1; i < sample_num_ / 2; ++i)
    {
        norm_sqr = fftr_result_[i].i * fftr_result_[i].i + fftr_result_[i].r * fftr_result_[i].r;
        if (norm_sqr > max_norm_sqr)
        {
            max_idx = i;
            max_norm_sqr = norm_sqr;
        }
    }

    // Retrieve estimation from FFT result
    float mean = std::accumulate(sample_y.begin(), sample_y.end(), 0.f) / float(sample_num_);
    float sq_sum = std::inner_product(sample_y.begin(), sample_y.end(), sample_y.begin(), 0.0f);
    float stdev = std::sqrt(sq_sum / sample_num_ - mean * mean);
    float freq = float(max_idx) / (float(sample_num_) * avg_update_interval_);

    amp_ = std::sqrt(stdev) * std::sqrt(2.f);
    omega_ = 2.f * M_PI * freq;
    phase_ = 0.f;
    offset_ = mean;
}

void TrigFitting::nlsFitting()
{

    Problem problem;
    Solver::Summary summary;
    double amp = amp_, omega = omega_, phase = phase_, offset = offset_;

    for (int i = 0; i < sample_num_; ++i)
    {
        CostFunction *cost_function =
                new AutoDiffCostFunction<TrigResidual, 1, 1, 1, 1, 1>(
                        new TrigResidual(double(sample_t[i]), double(sample_y[i])));
        problem.AddResidualBlock(cost_function, nullptr, &amp, &omega, &phase, &offset);
    }

    Solve(ceres_options_, &problem, &summary);

    amp_ = float(amp);
    omega_ = float(omega);
    phase_ = float(phase);
    offset_ = float(offset);
}