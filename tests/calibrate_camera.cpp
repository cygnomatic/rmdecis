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


class CameraCalibrator {

public:
    Mat cam_mat, dist_coeffs, cali_cam_mat;
    Size img_size, cail_img_size;
    Rect roi;

    explicit CameraCalibrator(const Size &img_size) {
        this->img_size = img_size;
    }

    void calculateCaliCoeffs(const std::vector<Mat> &imgs, const Size &pattern_size, float side_len) {
        Mat rvecs, tvecs;
        std::vector<std::vector<Point2f>> img_pts_arr;
        std::vector<std::vector<Point3f>> obj_pts_arr;
        std::vector<Point2f> corners;
        std::vector<Point3f> obj_pts = calculateBoardCornerPos(pattern_size, side_len);

        TermCriteria criteria{TermCriteria::EPS | TermCriteria::COUNT, 30, 0.001};

        for (const Mat &img: imgs) {
            findChessboardCorners(img, pattern_size, corners);

            if (corners.empty()) {
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
        std::cout << "DistortCoeffs: \n" << dist_coeffs << std::endl;
        std::cout << "CailCamMatrix: \n" << cali_cam_mat << std::endl;
        std::cout << "VaildROIsize: \n" << cail_img_size << std::endl;
    }

    void saveCoeffs(const String &path, const String &info) const {
        cv::FileStorage fs(path, FileStorage::WRITE);

        fs << "Info" << info;
        fs << "CameraMatrix" << cam_mat;
        fs << "DistortCoeffs" << dist_coeffs;

        fs.release();
    }

private:

    static std::vector<Point3f> calculateBoardCornerPos(const Size &pattern_size, float side_len) {
        std::vector<Point3f> corner;
        for (int i = 0; i < pattern_size.height; ++i)
            for (int j = 0; j < pattern_size.width; ++j)
                corner.emplace_back(i * side_len, j * side_len, 0);
        return corner;
    }
};


int main() {

    std::vector<String> img_paths;
    std::vector<Mat> imgs;
    Mat img;

    glob("../../data/calib/*.jpg", img_paths, false);

    for (const String &img_path: img_paths) {
        resize(imread(img_path, IMREAD_GRAYSCALE), img, Size(1200, 900));
        imgs.push_back(img);
    }

    CameraCalibrator camera_calibrator(Size(1200, 900));
    camera_calibrator.calculateCaliCoeffs(imgs, Size(BOARD_ROW, BOARD_COLUMN), GRID_LEN);
    camera_calibrator.saveCoeffs("../../config/camera.yml", "Test calib");

    return 0;
}


