//
// Created by catslashbin on 22-11-13.
//

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <utility>

using namespace cv;

#define BOARD_ROW 10
#define BOARD_COLUMN 7
#define GRID_LEN 28.5


class CameraCalibrator
{
    std::string cali_coeffs_path;

public:
    Mat cam_mat, dist_coeffs, cali_cam_mat;
    Size img_size, cail_img_size;
    Rect roi;

    explicit CameraCalibrator(std::string coeffs_path, const Size& img_size)
    {
        this->cali_coeffs_path = std::move(coeffs_path);
        this->img_size = img_size;
    }

    Mat undistort(const Mat &img) const
    {
        Mat ret;
        cv::undistort(img, ret, cam_mat, dist_coeffs);
        return ret;
    }

    void calculateCaliCoeffs(const std::vector<Mat> &imgs, const Size &pattern_size, float side_len)
    {
        Mat rvecs, tvecs;
        std::vector<std::vector<Point2f>> img_pts_arr;
        std::vector<std::vector<Point3f>> obj_pts_arr;
        std::vector<Point2f> corners;
        std::vector<Point3f> obj_pts = calculateBoardCornerPos(pattern_size, side_len);

        TermCriteria criteria{TermCriteria::EPS | TermCriteria::COUNT, 30, 0.001};

        for (const Mat &img: imgs)
        {
            findChessboardCorners(img, pattern_size, corners);

            if (corners.empty())
            {
                std::cerr << "No corner found." << std::endl;
                continue;
            }

            cornerSubPix(img, corners, Size(11, 11), Size(-1, -1), criteria);
            img_pts_arr.push_back(corners);
            obj_pts_arr.push_back(obj_pts);

            // drawChessboardCorners(img, pattern_size, corners, true);
            // imshow("Corners", img);
            // waitKey(30);

        }

        calibrateCamera(obj_pts_arr, img_pts_arr, img_size,
                        cam_mat, dist_coeffs, rvecs, tvecs);

        cali_cam_mat = getOptimalNewCameraMatrix(cam_mat, dist_coeffs, img_size, 1, img_size, &roi);
        cail_img_size = roi.size();

        std::cout << "CameraMatrix: \n" << cam_mat << std::endl;
        std::cout << "DistCoeffs: \n" << dist_coeffs << std::endl;
        std::cout << "CailCamMatrix: \n" << cali_cam_mat << std::endl;
        std::cout << "VaildROIsize: \n" << cail_img_size << std::endl;

        saveCoeffs();
    }

private:

    // bool loadCoeffs()
    // {
    //     cv::FileStorage fs(cail_coeffs_path, FileStorage::READ);
    //     if (fs.isOpened())
    //     {
    //         std::cerr << "Can not open file:" << cail_coeffs_path << std::endl;
    //         return false;
    //     }
    //     FileNode node = fs.getFirstTopLevelNode();
    //
    //     node["CameraMatrix"] >> cam_mat;
    //     node["DistCoeffs"] >> dist_coeffs;
    //     return true;
    // }

    void saveCoeffs()
    {
        cv::FileStorage fs(cali_coeffs_path, FileStorage::WRITE);

        fs << "OriImgSize" << img_size;
        fs << "CameraMatrix" << cam_mat;
        fs << "DistCoeffs" << dist_coeffs;
        fs << "ROI" << roi;
        fs << "CaliCamMat" << cali_cam_mat;

        fs.release();
    }

    static std::vector<Point3f> calculateBoardCornerPos(const Size &pattern_size, float side_len)
    {
        std::vector<Point3f> corner;
        for (int i = 0; i < pattern_size.height; ++i)
            for (int j = 0; j < pattern_size.width; ++j)
                corner.emplace_back(i * side_len, j * side_len, 0);
        return corner;
    }
};


int main()
{

    std::vector<String> img_paths;
    std::vector<Mat> imgs;
    Mat img;

    glob("../data/*.jpg", img_paths, false);

    for (const String &img_path: img_paths)
    {
        resize(imread(img_path, IMREAD_GRAYSCALE), img, Size(1200, 900));
        imgs.push_back(img);
    }

    CameraCalibrator camera_calibrator("../config/cam_cali_coeffs.yml", Size(1200, 900));
    camera_calibrator.calculateCaliCoeffs(imgs, Size(BOARD_ROW, BOARD_COLUMN), GRID_LEN);

    return 0;
}


