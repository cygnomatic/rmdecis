//
// Created by catslashbin on 22-11-16.
//

#ifndef CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H
#define CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <utility>
#include <iostream>
#include "objects/armor.h"

using namespace cv;

struct CameraCoeffs
{
    Size img_size;
    Mat cam_mat;
    Mat dist_coeffs;
    Rect roi;
    Mat cali_cam_mat;
};

struct CamExtrinsicParam
{
    Mat rvec = Mat::eye(3, 3, CV_32F);
    Mat tvec = Mat::zeros(3, 1, CV_32F);
};

class Cam3DReconstructor
{
    // CameraCoeffs cam_coeffs;
    Size img_size;
    Mat cam_mat;
    Mat dist_coeffs;
    Rect roi;
    Mat cali_cam_mat;

public:
    /**
     * Instantiate Cam3DReconstructor with camera coeffs.
     * @param cam_coeffs camera coeffs.
     */
    explicit Cam3DReconstructor(const std::string &coeffs_path);

    /**
     * Instantiate Cam3DReconstructor with coeffs in a .yml file.
     * @param cam_coeffs The file contains camera coeffs.
     */
    explicit Cam3DReconstructor(const CameraCoeffs &cam_coeffs);

    /**
     * Undistort the img.
     * @param img original img input.
     * @return undistorted img.
     */
    Mat undistort(const Mat &img);

    /**
     * Solve PNP with camera coeffs.
     * @param obj_pts
     * @param img_pts
     * @return Points in camera coordinate.
     */
    std::vector<Point3f> solvePNP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts);

    /**
     * Solve the armor position in camera coordinate.
     * @param armor
     * @param img_pts
     */
    void armorSolvePNP(Armor &armor, const std::vector<Point2f> &img_pts);
};


#endif //CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H
