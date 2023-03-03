//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_TYPING_GENERAL_H
#define CYGNOIDES_TYPING_GENERAL_H

#include <opencv2/core.hpp>

#include "consts.h"

/**
 * Type of time.
 * @param sec POSIX time, second.
 * @param usec POSIX time, microsecond.
 */
struct Time {
    int sec;
    int usec;

    bool isDefined = false;

    Time(int sec, int usec);

    Time();

    /**
     * Calculate the diff of two time.
     * @param rhs
     * @return Time difference, in second. If not time is not defined, return 0.
     */
    float operator-(const Time &rhs) const;

    /**
     * Add sec to the time instance.
     * @param rhs Time addition in second.
     * @return
     */
    Time operator+(float rhs) const;
};

struct EulerAngles {
    float yaw, pitch, roll;

    explicit EulerAngles() = default;

    explicit EulerAngles(const float angles[]);

    explicit EulerAngles(float yaw, float pitch, float roll);

    explicit EulerAngles(float yaw, float pitch);
};

struct CvTransform3f {
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);

    CvTransform3f() = default;

    /**
     * @param rvec In opencv standard
     * @param tvec
     */
    CvTransform3f(cv::Mat rvec, cv::Mat tvec);

    std::vector<cv::Point3f> applyTo(std::vector<cv::Point3f> pts) const;

    cv::Point3f applyTo(cv::Point3f pt) const;
};


struct RobotState {
    float gimbal_yaw, gimbal_pitch;
    float ballet_init_speed;

    /**
     * Current robot state input
     * @param gimbal_yaw in degree
     * @param gimbal_pitch in degree
     * @param ballet_init_speed in mm/s
     */
    explicit RobotState(float gimbal_yaw, float gimbal_pitch, float ballet_init_speed);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Corners2fImpl {

    Corners2fImpl() = default;

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @param corners Corners. Start form Top-Right, goes counter-clockwise to Down-Right.
     */
    explicit Corners2fImpl(std::vector<cv::Point2f> corners);

    cv::Point2f &operator[](int index);

    explicit operator std::vector<cv::Point2f>() const;

    size_t size();

    /**
     * Get the bounding box of Armor corners.
     * @return The bounding rect.
     */
    virtual cv::Rect2f getBoundingBox() const = 0;

protected:
    std::vector<cv::Point2f> corners_;
};

/**
 * Struct for the four corners of an Armor.
 * Corners coord in 2D.
 */
struct RectCorners2f : Corners2fImpl {

    using Corners2fImpl::Corners2fImpl;

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @note The corner points will be "moved" to this structure. Do not reuse original points.
     * @param tr Top-Right corner
     * @param tl Top-Left corner
     * @param dl Down-Right corner
     * @param dr Down-Left corner
     */
    explicit RectCorners2f(cv::Point2f tr, cv::Point2f tl, cv::Point2f dl, cv::Point2f dr);

    cv::Rect2f getBoundingBox() const override;
};

struct RuneCorners2f : Corners2fImpl {

    using Corners2fImpl::Corners2fImpl;

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @note The corner points will be "moved" to this structure. Do not reuse original points.
     */
    explicit RuneCorners2f(cv::Point2f center, cv::Point2f dr, cv::Point2f tr,
                           cv::Point2f top, cv::Point2f tl, cv::Point2f dl);

    cv::Rect2f getBoundingBox() const override;
};

struct Corners2f {
public:
    Corners2f() = default;

    Corners2f(RectCorners2f corners);

    Corners2f(RuneCorners2f corners);

    cv::Point2f &operator[](int index);

    operator std::vector<cv::Point2f>() const;

    size_t size();

    cv::Rect2f getBoundingBox() const;

private:
    std::shared_ptr<Corners2fImpl> impl;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Corners3fImpl {
    explicit Corners3fImpl() = default;

    explicit Corners3fImpl(std::vector<cv::Point3f> corners);

    cv::Point3f &operator[](int index);

    explicit operator std::vector<cv::Point3f>() const;

protected:
    std::vector<cv::Point3f> corners_;
};

struct RectCorners3f : Corners3fImpl {
    using Corners3fImpl::Corners3fImpl;

    /**
     * Construct a new ArmorCorners3d object. The center of the Armor is the origin.
     */
    explicit RectCorners3f(const cv::Size_<float> &armor_size);
};

struct SmallRectCorners3f : RectCorners3f {
    using RectCorners3f::RectCorners3f;

    explicit SmallRectCorners3f();
};

struct LargeRectCorners3f : RectCorners3f {
    using RectCorners3f::RectCorners3f;

    explicit LargeRectCorners3f();
};

struct RuneCorners3f : Corners3fImpl {
    using Corners3fImpl::Corners3fImpl;

    explicit RuneCorners3f();
};

struct Corners3f {
public:

    Corners3f() = default;

    Corners3f(SmallRectCorners3f corners);

    Corners3f(LargeRectCorners3f corners);

    Corners3f(RuneCorners3f corners);

    cv::Point3f &operator[](int index);

    operator std::vector<cv::Point3f>() const;

private:
    std::shared_ptr<Corners3fImpl> impl;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * The detect result of armor.
 */
struct DetectArmorInfo {

    FacilityID facility_id = UNKNOWN_BOT;
    Corners2f corners_img;
    float detection_confidence = 1.0;

    explicit DetectArmorInfo() = default;

    /**
     * Construct a struct to save the detection result of an armor.
     * @param facility_id Facility ID
     * @param corners_img Armor corners
     * @param detection_confidence Detection confidence
     */
    explicit DetectArmorInfo(FacilityID facility_id, Corners2f corners_img,
                       float detection_confidence);
};

struct DetectVaneInfo {
    bool is_target = true;
    Corners2f corners_img;
    float detection_confidence = 1.0;

    explicit DetectVaneInfo() = default;

    /**
     * Construct a struct to save the detection result of an rune armor.
     * @param is_target
     * @param corners_img Vane corners
     * @param detection_confidence Detection confidence
     */
    explicit DetectVaneInfo(bool is_target, Corners2f corners_img, float detection_confidence);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct ArmorFrameInput {
    int seq_idx = -1;

    Time time;
    RobotState robot_state;
    std::vector<DetectArmorInfo> armor_info;

    /**
     * The output of the Vision Armor detection.
     * @param seq_idx For test only. Index of the corresponding frame.
     * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
     * @param robot_state State info of the robot.
     * @param armor_info Detected armor info.
     * @note armor_info is moved into the struct. Do not reuse it.
     */
    ArmorFrameInput(int seq_idx, Time time, RobotState robot_state, std::vector<DetectArmorInfo> armor_info);
};


#endif //CYGNOIDES_TYPING_GENERAL_H
