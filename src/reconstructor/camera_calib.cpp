//
// Created by catslashbin on 22-11-16.
//

#include "camera_calib.h"
#include "utils/cv_utils.h"

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

CvTransform3f CameraCalib::solvePnP(const std::vector<cv::Point3f> &obj_pts, const std::vector<cv::Point2f> &img_pts) {
    Mat rvec, tvec;
    cv::solvePnP(obj_pts, img_pts, cam_mat, dist_coeffs, rvec, tvec);
    return CvTransform3f{rvec, tvec};
}

CvTransform3f CameraCalib::armorSolvePnP(const Corners3f& corners_model, const Corners2f& corners_img) {
    return solvePnP((std::vector<Point3f>) corners_model, (std::vector<Point2f>) corners_img);
}

void CameraCalib::drawAxes(Mat &img, const CvTransform3f &trans) {
    drawFrameAxes(img, cam_mat, dist_coeffs, trans.rvec, trans.tvec, 100);
}

std::vector<Point2f> CameraCalib::projectToImage(std::vector<Point3f> pts_cam_rep) {
    std::vector<Point2f> ret;
    for (auto &pt: pts_cam_rep) {
        pt = repToOpencv(pt);
    }
    projectPoints(pts_cam_rep, cv::Mat::zeros(3, 1, CV_64F), cv::Mat::zeros(3, 1, CV_64F), cam_mat, dist_coeffs, ret);
    return ret;
}

Point2f CameraCalib::projectToImage(Point3f pt_cam_rep) {
    std::vector<Point3f> inp({pt_cam_rep});
    return projectToImage(inp)[0];
}