//
// Created by catslashbin on 22-11-16.
//

#ifndef CYGNOIDES_DECISION_CAMERA_CALIB_H
#define CYGNOIDES_DECISION_CAMERA_CALIB_H

#include <utility>
#include <iostream>

#include "rmdecis/core.h"

class CameraCalib {
    // CameraCoeffs cam_coeffs;
    cv::Mat cam_mat, dist_coeffs;

public:

    /**
     * Instantiate Cam3DReconstructor with coeffs in a .yml file.
     * @param cam_coeffs The file contains camera coeffs.
     */
    explicit CameraCalib(Config &cfg);

    /**
     * Undistort the img.
     * @param img original img input.
     * @return undistorted img.
     */
    cv::Mat undistort(const cv::Mat &img);

    CvTransform3f solvePnP(const std::vector<cv::Point3f> &obj_pts, const std::vector<cv::Point2f> &img_pts);

    CvTransform3f armorSolvePnP(const ArmorCorners3d &corners_model, const ArmorCorners2d &corners_img);

    void drawAxes(cv::Mat &img, const CvTransform3f &trans);

    std::vector<cv::Point2f> projectToImage(const std::vector<cv::Point3f> &space_pts, const CvTransform3f &trans = {});
};


#endif //CYGNOIDES_DECISION_CAMERA_CALIB_H
