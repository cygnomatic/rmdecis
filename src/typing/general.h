//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_TYPING_GENERAL_H
#define CYGNOIDES_TYPING_GENERAL_H

#include <iostream>
#include <utility>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "armor.h"

using namespace cv;
using namespace spdlog;

/**
 * Type of time.
 * @param sec POSIX time, second.
 * @param usec POSIX time, microsecond.
 */
struct Time {
    int sec;
    int usec;

    /**
     * Calculate the diff of two time.
     * @param rhs
     * @return Time difference, in second.
     */
    float operator-(const Time &rhs) const {
        return (sec - rhs.sec) + (usec - rhs.usec) * 1e-6f;
    }
};

/**
 * The detect result of armor.
 * @param armor_type Type of armor.
 * @param corners_img Detected armor corners in image coord.
 * @param confidence Confidence of the prediction.
 */
struct DetectArmorInfo {
    ArmorID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float confidence = 0.0;
};

/**
 * The output of the Vision Armor detection.
 * @param seq_idx For test only. Index of the corresponding frame.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_info Detected armor info.
 */
struct DetectArmorResult {
    int seq_idx = -1;
    Time time{};
    std::vector<DetectArmorInfo> armor_info{};
};

/**
 * [DEPRECATED] The output of the Vision Part.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_type Type of armor.
 * @param corners_img_coord Detected armor corners in image coord.
 * @param confidence Confidence of the prediction.
 * @deprecated This struct is DEPRECATED. Use DetectArmorResult instead.
 */
struct ArmorPredResult {
    Time time{};
    ArmorID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float confidence{};
};

struct AxisAngles3f {
    float roll, pitch, yaw;

    explicit AxisAngles3f(const float angles[]) {
        roll = angles[0];
        pitch = angles[1];
        yaw = angles[2];
    }
};

struct AxisAngles2f {
    float pitch, yaw;

    AxisAngles2f() = default;

    explicit AxisAngles2f(const float angles[]) {
        pitch = angles[0];
        yaw = angles[1];
    }

    explicit AxisAngles2f(float pitch, float yaw) {
        this->pitch = pitch;
        this->yaw = yaw;
    }
};

struct Transform3d {
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);

    Transform3d() = default;

    Transform3d(cv::Mat rvec, cv::Mat tvec) {
        this->rvec = std::move(rvec);
        this->tvec = std::move(tvec);
    }

    std::vector<Point3f> applyTo(const std::vector<cv::Point3f> &pts) const {
        Mat rot_mat;
        std::vector<Point3f> ret;
        Rodrigues(rvec, rot_mat);

        cv::transform(pts, ret, rot_mat);

        for (auto &pt: pts) {
            ret.push_back((Point3f) tvec + pt);
        }

        return ret;
    }
};


#endif //CYGNOIDES_TYPING_GENERAL_H
