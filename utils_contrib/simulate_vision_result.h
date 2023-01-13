//
// Created by catslashbin on 23-1-4.
//

#ifndef CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
#define CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H

#include <string>
#include <yaml-cpp/yaml.h>

#include "../src/typing/general.h"

class SimulateVisionOutput {
    int next_idx = 0;
    std::map<int, DetectArmorResult> data;

public:

    explicit SimulateVisionOutput(const std::string &data_path);

    DetectArmorResult getData(int index);

    DetectArmorResult getNextData();
};

#endif //CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
