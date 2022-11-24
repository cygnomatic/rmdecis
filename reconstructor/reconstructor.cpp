//
// Created by catslashbin on 22-11-16.
//

#include "reconstructor.h"

Reconstructor::Reconstructor(const CameraCoeffs &cam_coeffs)
{
    img_size = cam_coeffs.img_size;
    cam_mat = cam_coeffs.cam_mat;
}

Reconstructor::Reconstructor(const std::string &coeffs_path)
{
    cv::FileStorage fs(coeffs_path, FileStorage::READ);
    if (fs.isOpened())
    {
        throw std::runtime_error("Can not open file: " + coeffs_path);
    }

    FileNode node = fs.getFirstTopLevelNode();

    node["OriImgSize"] >> img_size;
    node["CameraMatrix"] >> cam_mat;
}

Mat Reconstructor::undistort(const Mat &img)
{
    Mat ret;
    cv::undistort(img, ret, cam_mat, dist_coeffs, cam_mat);
    return ret;
}

std::vector<Point3f>
Reconstructor::solve_PNP(const std::vector<Point3f> &obj_pts, const std::vector<Point2f> &img_pts)
{
    Mat rvec, tvec;

    cv::solvePnP(obj_pts, img_pts, cam_mat, dist_coeffs, rvec, tvec);

    Mat cam_coord_pts = rvec * Mat(obj_pts) + tvec;
    std::vector<Point3f> ret;
    cam_coord_pts.copyTo(ret);

    return ret;
}


ArmorCorners3d Reconstructor::armor_solve_PNP(const ArmorCorners3d &corners_self_coord, const ArmorCorners2d &corners_img_coord)
{
    return ArmorCorners3d(solve_PNP((std::vector<Point3f>) corners_self_coord, (std::vector<Point2f>) corners_img_coord));
}
