//
// Created by catslashbin on 23-1-4.
//

#include "simulate_vision_result.h"

using namespace cv;

SimulateVisionOutput::SimulateVisionOutput(const std::string &data_path) {

    info("Start to load data from {} ...", data_path);

    std::vector<YAML::Node> ys = YAML::LoadAllFromFile(data_path);

    int seq_idx = 0;
    for (YAML::Node y: ys) {
        try {
            seq_idx = y["header"]["seq"].as<int>();

            YAML::Node stamp = y["header"]["stamp"];
            Time time = {stamp["secs"].as<int>(), stamp["nsecs"].as<int>() / (int) 1e3};

            std::vector<DetectArmorInfo> armor_info;

            for (YAML::Node d: y["data"]) {
                ArmorID armor_type = (ArmorID) d["armor_type"].as<int>();
                float confidence = d["confidence"].as<float>();

                Point2f rt{d["rt_x"].as<float>(), d["rt_y"].as<float>()};
                Point2f lt{d["lt_x"].as<float>(), d["lt_y"].as<float>()};
                Point2f ld{d["ld_x"].as<float>(), d["ld_y"].as<float>()};
                Point2f rd{d["rd_x"].as<float>(), d["rd_y"].as<float>()};
                ArmorCorners2d corners{{rt, lt, ld, rd}};

                armor_info.push_back(DetectArmorInfo{armor_type, corners, confidence});
            }

            data[seq_idx] = {seq_idx, time, armor_info};

            debug("Loaded {}th data", seq_idx);
        }
        catch (YAML::BadConversion &e) {
            warn("Failed to load {}th data: {}", ++seq_idx, e.msg);
        }
    }

    info("Successfully loaded {} data.", ys.size());
}

DetectArmorResult SimulateVisionOutput::getNextData() {
    auto d = getData(next_idx);
    next_idx++;
    return d;
}

DetectArmorResult SimulateVisionOutput::getData(int seq_idx) {
    auto i = data.find(seq_idx);
    if (i == data.end())
        return DetectArmorResult{};
    return data[seq_idx];
}
