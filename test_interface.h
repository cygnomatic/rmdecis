//
// Created by catslashbin on 22-11-23.
//

#ifndef CYGNOIDES_DECISION_TEST_INTERFACE_H
#define CYGNOIDES_DECISION_TEST_INTERFACE_H

#include "typing/general.h"
#include "reconstructor/camera_calib.h"
#include "utils/construct_armor.h"

static CameraCalib reconstructor("cam_cali_coeffs.yml");

float test_calc_armor_distance(const ArmorCorners2d& corners_img_coord)
{
    reconstructor.armor_solve_PNP(get_armor_self_coord(BIG_ARMOR), corners_img_coord);
}

#endif //CYGNOIDES_DECISION_TEST_INTERFACE_H
