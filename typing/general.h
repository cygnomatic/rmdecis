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
#include <spdlog/sinks/stdout_sinks.h>

#include "armor.h"

using namespace cv;
using namespace spdlog;

/**
 * Type of time.
 * @param sec POSIX time, second.
 * @param usec POSIX time, microsecond.
 */
struct Time
{
    int sec;
    int usec;

    /**
     * Calculate the diff of two time.
     * @param rhs
     * @return Time difference, in second.
     */
    float operator-(const Time &rhs) const
    {
        return (sec - rhs.sec) + (usec - rhs.usec) * 1e-6f;
    }
};

/**
 * The output of the Vision Part.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_type Type of armor.
 * @param corners_img_coord Detected armor corners in image coord.
 * @param confidence Confidence of the prediction.
 */
struct ArmorPredResult
{
    Time time{};
    ArmorID armor_type = UNKNOWN;
    ArmorCorners2d corners_img_coord;
    float confidence{};
};

struct AxisAngles3f
{
    float roll, pitch, yaw;

    explicit AxisAngles3f(const float angles[])
    {
        roll = angles[0];
        pitch = angles[1];
        yaw = angles[2];
    }
};

struct AxisAngles2f
{
    float pitch, yaw;

    AxisAngles2f() = default;

    explicit AxisAngles2f(const float angles[])
    {
        pitch = angles[0];
        yaw = angles[1];
    }

    explicit AxisAngles2f(float pitch, float yaw)
    {
        this->pitch = pitch;
        this->yaw = yaw;
    }
};

struct Transform3f
{
    cv::Mat rvec = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32F);

    Transform3f() = default;

    Transform3f(cv::Mat rvec, cv::Mat tvec)
    {
        this->rvec = std::move(rvec);
        this->tvec = std::move(tvec);
    }

    // std::vector<cv::Point3f> apply(const std::vector<cv::Point3f> &pts) const
    // {
    //     cv::Mat inp(pts);
    //     std::vector<cv::Point3f> ret;
    //
    //     cv::Mat tmp = rvec * inp + tvec;
    //
    //     tmp.copyTo(ret);
    //     return ret;
    // }
};


#endif //CYGNOIDES_TYPING_GENERAL_H
