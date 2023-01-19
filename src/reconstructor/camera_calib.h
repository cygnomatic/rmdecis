//
// Created by catslashbin on 22-11-16.
//

#ifndef CYGNOIDES_DECISION_CAMERA_CALIB_H
#define CYGNOIDES_DECISION_CAMERA_CALIB_H

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <utility>
#include <iostream>

#include "../typing/core.h"
#include "general.h"

struct CameraCoeffs {
    cv::Mat cam_mat;
    cv::Mat dist_coeffs;
};

class CameraCalib {
    // CameraCoeffs cam_coeffs;
    cv::Mat cam_mat, dist_coeffs;

public:
    /**
     * Instantiate Cam3DReconstructor with camera coeffs.
     * @param cam_coeffs camera coeffs.
     */
    explicit CameraCalib(const std::string &coeffs_path);

    /**
     * Instantiate Cam3DReconstructor with coeffs in a .yml file.
     * @param cam_coeffs The file contains camera coeffs.
     */
    explicit CameraCalib(const CameraCoeffs &cam_coeffs);

    /**
     * Undistort the img.
     * @param img original img input.
     * @return undistorted img.
     */
    cv::Mat undistort(const cv::Mat &img);

    Transform3d solvePnP(const std::vector<cv::Point3f> &obj_pts, const std::vector<cv::Point2f> &img_pts);

    Transform3d armorSolvePnP(const ArmorCorners3d &corners_model, const ArmorCorners2d &corners_img);

    void drawAxes(cv::Mat &img, const Transform3d &trans);

    std::vector<cv::Point2f> projectToImage(const std::vector<cv::Point3f> &space_pts, const Transform3d &trans = {});
};


#endif //CYGNOIDES_DECISION_CAMERA_CALIB_H
