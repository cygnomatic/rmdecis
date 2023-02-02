#include <kiss_fftr.h>
#include <cassert>
#include <vector>

int main() {
    std::vector<float> data = {1.95174714, 1.98113303, 2.15044697, 1.57042874, 0.89841985, 0.73438475,
                    0.66102871, 1.10053382, 1.00486745, 1.32932005, 1.84976536, 2.06622601,
                    2.1297348, 1.81640903, 1.72603164, 1.633};

    assert(data.size() % 2 == 0);

    kiss_fft_cpx out[100] = {};

    auto cfg = kiss_fftr_alloc((int)data.size(), 0, nullptr, nullptr);
    kiss_fftr(cfg, data.data(), out);

    return 0;
}