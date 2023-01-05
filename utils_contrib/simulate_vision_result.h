//
// Created by catslashbin on 23-1-4.
//

#ifndef CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
#define CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H

#include <string>
#include <yaml-cpp/yaml.h>

#include "../src/typing/general.h"

class SimulateVisionOutput
{
    std::vector<YAML::Node> data;

public:

    explicit SimulateVisionOutput(const std::string &data_path);

    std::vector<ArmorPredResult> getData(std::size_t index);

};

#endif //CYGNOIDES_DECISION_SIMULATE_VISION_RESULT_H
