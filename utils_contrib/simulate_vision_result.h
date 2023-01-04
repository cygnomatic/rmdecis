//
// Created by catslashbin on 23-1-4.
//

#ifndef CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
#define CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H

#include <string>
#include <yaml-cpp/yaml.h>

#include "../typing/general.h"

class SimulateVisionOutput
{
    std::vector<YAML::Node> data;
    size_t curr_idx = 0;

public:

    explicit SimulateVisionOutput(const std::string &data_path);

    ArmorPredResult getData(std::size_t index);

    ArmorPredResult nextData();

};

#endif //CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
