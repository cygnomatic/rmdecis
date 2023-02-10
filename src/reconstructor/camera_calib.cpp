//
// Created by catslashbin on 22-11-16.
//

#include "camera_calib.h"

using namespace cv;

CameraCalib::CameraCalib(Config& cfg) {

    auto coeffs_path = cfg.get<std::string>("camera.coeffsPath");
    cv::FileStorage fs(coeffs_path, cv::FileStorage::READ);

    if (!fs.isOpened()) {
        throw std::runtime_error("Can not open file: " + coeffs_path);
    }

    fs["CameraMatrix"] >> cam_mat;
    fs["DistortCoeffs"] >> dist_coeffs;

    fs.release();

    info("Loaded camera coeffs. ");
}

Mat CameraCalib::undistort(const Mat &img) {
    Mat ret;
    cv::undistort(img, ret, cam_mat, dist_coeffs, cam_mat);
    return ret;
}

CvTransform3f CameraCalib::solvePnP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts) {
    Mat rvec, tvec;
    cv::solvePnP(Mat(obj_pts), Mat(img_pts), cam_mat, dist_coeffs, rvec, tvec);
    return CvTransform3f{rvec, tvec};
}

CvTransform3f CameraCalib::armorSolvePnP(const ArmorCorners3d &corners_model, const ArmorCorners2d &corners_img) {
    return solvePnP((std::vector<Point3f>) corners_model, (std::vector<Point2f>) corners_img);
}

void CameraCalib::drawAxes(Mat &img, const CvTransform3f &trans) {
    drawFrameAxes(img, cam_mat, dist_coeffs, trans.rvec, trans.tvec, 100);
}

std::vector<Point2f> CameraCalib::projectToImage(const std::vector<Point3f> &space_pts, const CvTransform3f &trans) {
    Mat img_pts;
    std::vector<Point2f> ret;

    projectPoints(Mat(space_pts), trans.rvec, trans.tvec, cam_mat, dist_coeffs, img_pts);

    img_pts.copyTo(ret);
    return ret;
}