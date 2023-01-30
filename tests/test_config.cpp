//
// Created by catslashbin on 23-1-29.
//

#include "utils/config.h"

int main() {
    auto cl = ConfigLoader("../../config");
    auto cfg = cl.load("kalman");
    info("processNoiseCov, SD: {}", cfg.get<float>("processNoiseCov.SD"));
    info("processNoiseCov, SD: {}", cfg.get<float>("processNoiseCov.SD"));
    info("processNoiseCov, NOTHING: {}", cfg.get<float>("processNoiseCov.N.O.T.H.I.N.G", 0.1));
}