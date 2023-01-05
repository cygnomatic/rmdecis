//
// Created by catslashbin on 23-1-4.
//
#include "simulate_vision_result.h"

SimulateVisionOutput::SimulateVisionOutput(const std::string &data_path) {
    data = YAML::LoadAllFromFile(data_path);
    info("Data size: {}", data.size());
}

std::vector<ArmorPredResult> SimulateVisionOutput::getData(std::size_t index)
{
    if (index >= data.size())
        return {};

    YAML::Node stamp = data[index]["header"]["stamp"];
    Time time = {stamp["secs"].as<int>(), stamp["nsecs"].as<int>() / (int) 1e3};

    std::vector<ArmorPredResult> ret;

    for (YAML::Node d: data[index]["data"])
    {
        ArmorID armor_type = (ArmorID) d["armor_type"].as<int>();
        float confidence = d["confidence"].as<float>();
        Point2f rt{d["rt_x"].as<float>(), d["rt_y"].as<float>()};
        Point2f lt{d["lt_x"].as<float>(), d["lt_y"].as<float>()};
        Point2f ld{d["ld_x"].as<float>(), d["ld_y"].as<float>()};
        Point2f rd{d["rd_x"].as<float>(), d["rd_y"].as<float>()};
        ArmorCorners2d corners{{rt, lt, ld, rd}};

        info("header.seq: {}, frame_pos {}", data[index]["header"]["seq"].as<int>(), index);
        ret.push_back(ArmorPredResult{time, armor_type, corners, confidence});
    }

    return ret;
}
