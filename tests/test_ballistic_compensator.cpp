//
// Created by catslashbin on 23-4-1.
//

#include "compensator/ballistic_compensator.h"
#include "rmdecis/core.h"

int main() {

    Config cfg("../config/config.yml");
    BallisticCompensator ballistic_compensator(cfg);

    info(ballistic_compensator.calcShootAngle(5500, 800, 15000, -0.21));

    return 0;
}