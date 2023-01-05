//
// Created by catslashbin on 22-11-27.
//

#include "transformer.h"


inline std::vector<Point3f> mat2pts(const Mat& mat)
{
    std::vector<Point3f> ret;
    mat.copyTo(ret);
    return ret;
}

std::vector<Point3f> Transformer::self2cam(const Transform3f &cam_ext, const std::vector<Point3f> &pts_self_coord)
{
    Mat pts_cam_coord = cam_ext.rvec * Mat(pts_self_coord) + cam_ext.tvec;
    return mat2pts(pts_cam_coord);
}

std::vector<Point3f> Transformer::cam2self(const Transform3f &cam_ext, const std::vector<Point3f> &pts_cam_coord)
{
    Mat pts_self_coord = cam_ext.rvec.inv() * (Mat(pts_cam_coord) - cam_ext.tvec);
    return mat2pts(pts_self_coord);
}

std::vector<Point3f> Transformer::cam2gimbal(const std::vector<Point3f> &pts_cam_coord, const Vec3f &shift)
{
    return mat2pts(Mat(pts_cam_coord) + shift);
}

// ArmorCorners3d Transformer::img2cam(CameraCalib &cam_calib, const ArmorCorners3d &corners_self_coord,
//                                         const ArmorCorners2d &corners_img_coord)
// {
//     return ArmorCorners3d(cam_calib.reconstruct((std::vector<Point3f>) corners_self_coord,
//                                                 (std::vector<Point2f>) corners_img_coord));
// }
//
// ArmorCorners3d Transformer::cam2gimbal(const ArmorCorners3d &corners_cam_coord, const Point3f &shift)
// {
//
//     return corners_cam_coord + shift;
// }
//
// SolveArmorResult Transformer::solve(ArmorCorners3d &corners_gimbal_coord)
// {
//     return SolveArmorResult();
// }
//
// SolveArmorResult Transformer::solve(CameraCalib &cam_calib, const ArmorCorners3d &corners_self_coord,
//                                     const ArmorCorners2d &corners_img_coord, const Vec3f &shift)
// {
//     return SolveArmorResult();
// }

