//
// Created by catslashbin on 23-1-4.
//
#include "simulate_vision_result.h"

ArmorPredResult SimulateVisionOutput::getData(std::size_t index)
{
    const YAML::Node &header = data[index]["header"];
    const YAML::Node &stamp = header["stamp"];

    Time time = {stamp["secs"].as<int>(), stamp["nsecs"].as<int>() / (int) 1e3};
    ArmorID armor_type = (ArmorID) data[index]["armor_type"].as<int>();
    float confidence = data[index]["confidence"].as<float>();

    Point2f rt{(data[index]["rt_x"].as<float>(), data[index]["rt_y"].as<float>())};
    Point2f lt{(data[index]["lt_x"].as<float>(), data[index]["lt_y"].as<float>())};
    Point2f ld{(data[index]["ld_x"].as<float>(), data[index]["ld_y"].as<float>())};
    Point2f rd{(data[index]["rd_x"].as<float>(), data[index]["rd_y"].as<float>())};
    ArmorCorners2d corners{{rt, lt, ld, rd}};

    return ArmorPredResult{time, armor_type, corners, confidence};
}

ArmorPredResult SimulateVisionOutput::nextData()
{
    return getData(curr_idx);
    curr_idx ++;
}
