//
// Created by catslashbin on 22-11-16.
//

#include "camera_calib.h"

CameraCalib::CameraCalib(const CameraCoeffs &cam_coeffs)
{
    img_size = cam_coeffs.img_size;
    cam_mat = cam_coeffs.cam_mat;
}

CameraCalib::CameraCalib(const std::string &coeffs_path)
{
    cv::FileStorage fs(coeffs_path, FileStorage::READ);
    if (!fs.isOpened())
    {
        throw std::runtime_error("Can not open file: " + coeffs_path);
    }

    FileNode node = fs.getFirstTopLevelNode();

    node["OriImgSize"] >> img_size;
    node["CameraMatrix"] >> cam_mat;
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

std::vector<Point3f>
CameraCalib::reconstruct(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts)
{
    Mat rvec, tvec;
    solvePnP(obj_pts, img_pts, rvec, tvec);

    Mat cam_coord_pts = rvec * Mat(obj_pts) + tvec;
    std::vector<Point3f> ret;
    cam_coord_pts.copyTo(ret);

    return ret;
}


ArmorCorners3d CameraCalib::armorSolvePnP(const ArmorCorners3d &corners_self_coord, const ArmorCorners2d &corners_img_coord)
{
    return ArmorCorners3d(
            reconstruct((std::vector<Point3f>) corners_self_coord, (std::vector<Point2f>) corners_img_coord));
}
