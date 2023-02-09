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

struct Transform3d {
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);

    Transform3d() = default;

    Transform3d(cv::Mat rvec, cv::Mat tvec);

    std::vector<cv::Point3f> applyTo(const std::vector<cv::Point3f> &pts) const;

    cv::Point3f applyTo(const cv::Point3f &pt) const;
};

/**
 * Struct for the four corners of an Armor.
 * Corners coord in 2D.
 */
struct ArmorCorners2d {
    ArmorCorners2d() = default;

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @param corners Corners. Start form Top-Right, goes counter-clockwise to Down-Right.
     */
    explicit ArmorCorners2d(cv::Point2f corners[]);

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @param corners Corners. Start form Top-Right, goes counter-clockwise to Down-Right.
     */
    explicit ArmorCorners2d(std::vector<cv::Point2f> corners);

    /**
     * Construct ArmorCorners2d with four corners coord.
     * @note The corner points will be "moved" to this structure. Do not reuse original points.
     * @param tr Top-Right corner
     * @param tl Top-Left corner
     * @param dl Down-Right corner
     * @param dr Down-Left corner
     */
    explicit ArmorCorners2d(cv::Point2f tr, cv::Point2f tl, cv::Point2f dl, cv::Point2f dr);

    cv::Point2f tr; // Top-Right
    cv::Point2f tl; // Top-Left
    cv::Point2f dl; // Down-Left
    cv::Point2f dr; // Down-Right

    cv::Point2f &operator[](int index);

    explicit operator std::vector<cv::Point2f>() const;

    /**
     * Get the bounding box of Armor corners.
     * @return The bounding rect.
     */
    explicit operator cv::Rect2f() const;

    /**
     * Get the bounding box of Armor corners.
     * @return The bounding rect.
     */
    cv::Rect2f getBoundingBox() const;

    std::vector<cv::Point2f> toPts();
};

struct ArmorCorners3d {
    ArmorCorners3d() = default;

    explicit ArmorCorners3d(cv::Point3f corners[]);

    explicit ArmorCorners3d(std::vector<cv::Point3f> corners);

    /**
     * Construct a new ArmorCorners3d object. The center of the Armor is the origin.
     * @param armor_size Armor size.
     */
    explicit ArmorCorners3d(const cv::Size_<float> &armor_size);

    /**
     * Construct a new ArmorCorners3d object. The center of the Armor is the origin and size is defined.
     * Used to construct armors in Model Coordinate.
     * @param armorType The armor type.
     */
    explicit ArmorCorners3d(ArmorType armorType);

    cv::Point3f tr; // Top-Right
    cv::Point3f tl; // Top-Left
    cv::Point3f dl; // Down-Left
    cv::Point3f dr; // Down-Right

    cv::Point3f &operator[](int index);

    explicit operator std::vector<cv::Point3f>() const;

    cv::Point3f getCenter() const;

};

/**
 * The detect result of armor.
 */
struct DetectArmorInfo {

    // Info from vision detection part
    FacilityID facility_id = UNKNOWN_BOT;
    ArmorType armorType = SMALL_ARMOR;

    ArmorCorners2d corners_img;
    ArmorCorners3d corners_model;
    float detection_confidence = 1.0;

    // Info form reconstructor
    cv::Point3f center_base;
    cv::Point3f center_cam;
    Transform3d trans_model2cam;
    float reconstruct_confidence = 0.0;

    explicit DetectArmorInfo() = default;

    /**
     * Construct a struct to save the detection result of an armor.
     * @param facility_id Facility ID
     * @param corners_img Armor corners
     * @param detection_confidence Detection confidence
     * @note corners_img is moved into the struct. Do not reuse it.
     */
    explicit DetectArmorInfo(FacilityID facility_id, ArmorCorners2d corners_img, float detection_confidence = 1.0);
};

struct DetectArmorsFrame {
    int seq_idx = -1;
    Time time{};
    std::vector<DetectArmorInfo> armor_info{};

    /**
     * The output of the Vision Armor detection.
     * @param seq_idx For test only. Index of the corresponding frame.
     * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
     * @param armor_info Detected armor info.
     * @note armor_info is moved into the struct. Do not reuse it.
     */
    explicit DetectArmorsFrame(int seq_idx, Time time, std::vector<DetectArmorInfo> armor_info);
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
    FacilityID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float confidence{};
};

struct TrackArmorInfo {
    cv::Point3f center_gimbal;
    cv::Rect2f bbox;
};


#endif //CYGNOIDES_TYPING_GENERAL_H
