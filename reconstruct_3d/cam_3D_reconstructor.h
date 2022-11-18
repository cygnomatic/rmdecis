//
// Created by catslashbin on 22-11-16.
//

#ifndef CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H
#define CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H

// #include <Eigen/Core>
// #include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <utility>
#include <iostream>
#include <utility>

using namespace cv;
// using namespace Eigen;

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
    explicit Cam3DReconstructor(const CameraCoeffs& cam_coeffs);
    /**
     * Instantiate Cam3DReconstructor with camera coeffs file.
     * @param coeffs_path Path to the coeffs file.
     */

    explicit Cam3DReconstructor(const std::string &coeffs_path);

    Mat undistort(const Mat &img);

    std::vector<Point3f> solvePNP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts);
};


#endif //CYGNOIDES_DECISION_CAM_3D_RECONSTRUCTOR_H
