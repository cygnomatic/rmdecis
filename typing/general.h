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
 * Struct for the four corners of an Armor.
 * Corners coord in 2D.
 */
struct ArmorCorners2d
{
    ArmorCorners2d() = default;

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @param corners Corners. Start form Top-Right, goes counter-clockwise to Down-Right.
     */
    explicit ArmorCorners2d(cv::Point2f corners[])
    {
        tr = corners[0];
        tl = corners[1];
        dl = corners[2];
        dr = corners[3];
    }

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @param corners Corners. Start form Top-Right, goes counter-clockwise to Down-Right.
     */
    explicit ArmorCorners2d(std::vector<cv::Point2f> corners)
    {
        assert(corners.size() == 4);
        tr = corners[0];
        tl = corners[1];
        dl = corners[2];
        dr = corners[3];
    }

    cv::Point2f tr; // Top-Right
    cv::Point2f tl; // Top-Left
    cv::Point2f dl; // Down-Left
    cv::Point2f dr; // Down-Right

    cv::Point2f &operator[](int index)
    {
        switch (index)
        {
            case 0:
                return tr;
            case 1:
                return tl;
            case 2:
                return dl;
            case 3:
                return dr;
            default:
                throw std::range_error("Corner index should be in 0 to 3.");
        }
    }

    explicit operator std::vector<cv::Point2f>() const
    {
        return std::vector<cv::Point2f>({tr, tl, dl, dr});
    }

    /**
     * Get the bounding box of Armor corners.
     * @return The bounding rect.
     */
    explicit operator Rect2f() const
    {
        return boundingRect(std::vector<cv::Point2f>({tr, tl, dl, dr}));
//        return {Point2f(min(tl.x, dl.x), min(dr.y, dl.y)), Point2f(max(tr.x, tl.x), max(tr.y, tl.y))};
    }

    //    cv::Point2f center() const
    //    {
    //        return (tr + tl + dl + dr) / 4;
    //    }
    //
    //    float area() const
    //    {
    //        return (float) (norm((tr - tl).cross(tr - dl)) + norm((dr - dl).cross(dr - tl)));
    //    }
    //
    //    float ratio() const
    //    {
    //        return (max(tr.y, tl.y) - min(dr.y, dl.y)) / (max(tr.x, tl.x) - min(tl.x, dl.x));
    //    }

};

struct ArmorCorners3d
{
    ArmorCorners3d() = default;

    explicit ArmorCorners3d(cv::Point3f corners[])
    {
        tr = corners[0];
        tl = corners[1];
        dl = corners[2];
        dr = corners[3];
    }

    explicit ArmorCorners3d(std::vector<cv::Point3f> corners)
    {
        assert(corners.size() == 4);
        tr = corners[0];
        tl = corners[1];
        dl = corners[2];
        dr = corners[3];
    }

    cv::Point3f tr; // Top-Right
    cv::Point3f tl; // Top-Left
    cv::Point3f dl; // Down-Left
    cv::Point3f dr; // Down-Right

    cv::Point3f &operator[](int index)
    {
        switch (index)
        {
            case 0:
                return tr;
            case 1:
                return tl;
            case 2:
                return dl;
            case 3:
                return dr;
            default:
                throw std::range_error("Corner index should be in 0 to 3.");
        }
    }

    explicit operator std::vector<cv::Point3f>() const
    {
        return std::vector<cv::Point3f>({tr, tl, dl, dr});
    }

    cv::Point3f getCenter() const
    {
        return (tr + tl + dl + dr) / 4;
    }

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
        this->rvec = rvec;
        this->tvec = tvec;
    }

    std::vector<cv::Point3f> apply(const std::vector<cv::Point3f> &pts) const
    {
        cv::Mat inp(pts);
        std::vector<cv::Point3f> ret;

        cv::Mat tmp = rvec * inp + tvec;

        tmp.copyTo(ret);
        return ret;
    }
};

static const int NUM_ARMOR_ID = 11;


enum ArmorID
{
    UNKNOWN = 0,

    HERO_1 = 1,

    ENGINEER_2 = 2,
    STANDARD_3 = 3,

    STANDARD_4 = 4,
    STANDARD_5 = 5,
    SENTRY_7 = 7,

    BASE_8 = 8,
    OUTPOST_10 = 10,
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
    Time time;
    ArmorID armor_type;
    ArmorCorners2d corners_img_coord;
    float confidence;
};

#endif //CYGNOIDES_TYPING_GENERAL_H
