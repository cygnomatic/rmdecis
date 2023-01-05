//
// Created by catslashbin on 22-11-16.
//

#include "camera_calib.h"

CameraCalib::CameraCalib(const CameraCoeffs &cam_coeffs)
{
    cam_mat = cam_coeffs.cam_mat;
    dist_coeffs = cam_coeffs.dist_coeffs;
}

CameraCalib::CameraCalib(const std::string &coeffs_path)
{
    cv::FileStorage fs(coeffs_path, FileStorage::READ);
    if (!fs.isOpened())
    {
        throw std::runtime_error("Can not open file: " + coeffs_path);
    }

    String calib_info;
    fs["Info"] >> calib_info;
    fs["CameraMatrix"] >> cam_mat;
    fs["DistortCoeffs"] >> dist_coeffs;

    fs.release();

    info("Loaded camera coeffs. CalibInfo: {}", calib_info);
}

Mat CameraCalib::undistort(const Mat &img)
{
    Mat ret;
    cv::undistort(img, ret, cam_mat, dist_coeffs, cam_mat);
    return ret;
}

void CameraCalib::solvePnP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts, Mat &rvec, Mat &tvec)
{
    cv::solvePnP(Mat(obj_pts), Mat(img_pts), cam_mat, dist_coeffs, rvec, tvec);
}

void CameraCalib::armorSolvePnP(const ArmorCorners3d &corners_self_coord, const ArmorCorners2d &corners_img_coord, Mat &rvec, Mat &tvec)
{
    solvePnP((std::vector<Point3f>) corners_self_coord, (std::vector<Point2f>) corners_img_coord, rvec, tvec);
}