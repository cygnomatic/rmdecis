//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_GENERAL_TYPING_H
#define CYGNOIDES_GENERAL_TYPING_H

#include <opencv2/core.hpp>

struct ArmorCorners2d
{
    ArmorCorners2d() = default;

    explicit ArmorCorners2d(cv::Point2f corners[])
    {
        tr = corners[0];
        tl = corners[1];
        dl = corners[2];
        dr = corners[3];
    }

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

    // cv::Point2f get_center() const
    // {
    //     return (tr + tl + dl + dr) / 4;
    // }
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

    // cv::Point3f get_center() const
    // {
    //     return (tr + tl + dl + dr) / 4;
    // }
};


#endif //CYGNOIDES_GENERAL_TYPING_H
