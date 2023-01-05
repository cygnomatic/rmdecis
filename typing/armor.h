//
// Created by catslashbin on 22-11-28.
//

#ifndef CYGNOIDES_DECISION_ARMOR_H
#define CYGNOIDES_DECISION_ARMOR_H

#include "general.h"

#define SMALL_ARMOR_WIDTH 135.0f
#define SMALL_ARMOR_HEIGHT 125.0f
#define BIG_ARMOR_WIDTH 230.0f
#define BIG_ARMOR_HEIGHT 127.0f

#define NUM_ARMOR_ID 11
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

enum ARMOR_TYPE
{
    SMALL_ARMOR = 0,
    BIG_ARMOR = 1
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
    explicit operator cv::Rect2f() const
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

    /**
     * Construct a new ArmorCorners3d object. The center of the Armor is the origin.
     * @param armor_size Armor size.
     */
    explicit ArmorCorners3d(const cv::Size_<float> &armor_size) :
            ArmorCorners3d(
                    {{armor_size.width / 2,  armor_size.height / 2,  0},
                     {-armor_size.width / 2, armor_size.height / 2,  0},
                     {-armor_size.width / 2, -armor_size.height / 2, 0},
                     {armor_size.width / 2,  -armor_size.height / 2, 0}}
            ) {}

    /**
     * Construct a new ArmorCorners3d object. The center of the Armor is the origin and size is defined.
     * Used to construct armors in Model Coordinate.
     * @param armorType The armor type.
     */
    explicit ArmorCorners3d(ARMOR_TYPE armorType)
    {
        switch (armorType)
        {
            case SMALL_ARMOR:
                *this = ArmorCorners3d({SMALL_ARMOR_WIDTH, SMALL_ARMOR_HEIGHT});
            case BIG_ARMOR:
                *this = ArmorCorners3d({BIG_ARMOR_WIDTH, BIG_ARMOR_HEIGHT});
        }
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

struct ArmorInfo
{
    ArmorID armor_id = UNKNOWN;
    ArmorCorners3d corners_cam;
    ArmorCorners2d corners_img;
};

#endif //CYGNOIDES_DECISION_ARMOR_H
