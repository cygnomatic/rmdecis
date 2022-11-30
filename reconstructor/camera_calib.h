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

#include "../typing/general.h"

using namespace cv;

struct CameraCoeffs
{
    Size img_size;
    Mat cam_mat;
    Mat dist_coeffs;
};

class CameraCalib
{
    // CameraCoeffs cam_coeffs;
    Size img_size;
    Mat cam_mat;
    Mat dist_coeffs;

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
    Mat undistort(const Mat &img);

    /**
     * Solve PNP with camera coeffs.
     * @param obj_pts
     * @param img_pts
     * @return Points in camera coordinate.
     */
    std::vector<Point3f> reconstruct(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts);

    /**
     * Solve the armor position in camera coordinate.
     * @param corners_self_coord Corners coordinate in itself coordinate
     * @param corners_img_coord Corners coordinate in image coordinate
     */
    ArmorCorners3d armor_solve_PNP(const ArmorCorners3d &corners_self_coord, const ArmorCorners2d &corners_img_coord);

    void solvePnP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts, Mat &rvec, Mat &tvec);
};


#endif //CYGNOIDES_DECISION_CAMERA_CALIB_H
