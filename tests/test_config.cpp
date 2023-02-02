//
// Created by catslashbin on 23-1-29.
//

#include "rmdecis_impl/config_impl.h"

int main() {
    auto cfg = Config("../../config/config.yml");
    info("processNoiseCov, SD: {}", cfg.get<float>("kalman.processNoiseCov.SD"));
    info("processNoiseCov, SD: {}", cfg.get<float>("kalman.processNoiseCov.SD"));
    info("processNoiseCov, NOTHING: {}", cfg.get<float>("processNoiseCov.N.O.T.H.I.N.G", 0.1));
}