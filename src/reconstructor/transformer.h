//
// Created by catslashbin on 23-1-31.
//

#ifndef CYGNOIDES_DECISION_TRANSFORMER_H
#define CYGNOIDES_DECISION_TRANSFORMER_H

#include "../../include/typing.h"
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>

class Transform;

class Frame;

class Transformer {

};

class Transform {
private:
    Eigen::AffineCompact3f transform_;

public:

    Frame &parent_;
    Frame &child_;

    Transform(Frame &parent, Frame &child);

    Transform(Frame &parent, Frame &child, const Eigen::Matrix3f &rot, const Eigen::Vector3f &trans);

    Transform(Frame &parent, Frame &child, const EulerAngles &rot, const Eigen::Vector3f &trans);

    Eigen::Vector3f applyTo(const Eigen::Vector3f& pt);

    Eigen::Vector3f applyInverseTo(const Eigen::Vector3f& pt);
};

class Frame {
private:
    std::string name;

public:
    explicit Frame(std::string name) : name(name) {}

    bool operator==(const Frame& other) const {
        return name == other.name;
    }
};


#endif //CYGNOIDES_DECISION_TRANSFORMER_H
