//
// Created by catslashbin on 22-11-21.
//

#include "armor_track.h"


class KalmanFilterFactory
{
public:
    static Mat getTransitionMatrix(float dt)
    {
        Mat transitionMatrix = (Mat_<float>(8, 8)
                <<
                1, 0, 0, 0, dt, 0, 0, 0,
                0, 1, 0, 0, 0, dt, 0, 0,
                0, 0, 1, 0, 0, 0, dt, 0,
                0, 0, 0, 1, 0, 0, 0, dt,
                0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 1
        );
        return transitionMatrix;
    }

    static Mat getMeasurementMat()
    {
        // State Matrix: [u, v, ratio, area, v_u, v_v, v_ration, v_area]
        return Mat::eye(8, 4, CV_32F);
    }

    static Mat getProcessNoiseCov(float dt, float SD, float SR, float SH)
    {
        Mat processNoiseCov = (Mat_<float>(8, 8)
                <<
                SD * dt * dt, 0, 0, 0, SD * dt, 0, 0, 0,
                0, SD * dt * dt, 0, 0, 0, SD * dt, 0, 0,
                0, 0, SR * dt * dt, 0, 0, 0, SR * dt, 0,
                0, 0, 0, SH * dt * dt, 0, 0, 0, SH * dt,
                0, 0, 0, 0, SD, 0, 0, 0,
                0, 0, 0, 0, 0, SD, 0, 0,
                0, 0, 0, 0, 0, 0, SR, 0,
                0, 0, 0, 0, 0, 0, 0, SH
        );
        return processNoiseCov;
    }

    static Mat getMeasurementNoiseCov(float dt, float SDM, float SRM, float SHM)
    {
        Mat measurementNoiseCov = (Mat_<float>(4, 4)
                <<
                SDM, 0, 0, 0,
                0, SDM, 0, 0,
                0, 0, SRM, 0,
                0, 0, 0, SHM
        );
        return measurementNoiseCov;
    }

};


void ArmorTrack::correct(const ArmorInfo &armor, float curr_time)
{
    auto dt = curr_time - last_pred_time;
    kf.transitionMatrix = KalmanFilterFactory::getTransitionMatrix(dt);
    // kf.measurementMatrix = KalmanFilterFactory::getMeasurementMat();
    kf.processNoiseCov = KalmanFilterFactory::getProcessNoiseCov(dt, 0.1, 0.1, 0.1);
    kf.measurementNoiseCov = KalmanFilterFactory::getMeasurementNoiseCov(dt, 0.1, 0.1, 0.1);

    Rect2f bounding_box = (Rect2f) armor.corners_img;
    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    kf.correct(Mat({u, v, ratio, area}));
    id_cnt[armor.armor_id]++;
}

Rect2f ArmorTrack::predict(float curr_time)
{
    auto dt = curr_time - last_pred_time;
    kf.transitionMatrix = KalmanFilterFactory::getTransitionMatrix(dt);
    kf.processNoiseCov = KalmanFilterFactory::getProcessNoiseCov(dt, 0.1, 0.1, 0.1);
    kf.measurementNoiseCov = KalmanFilterFactory::getMeasurementNoiseCov(dt, 0.1, 0.1, 0.1);
    Mat result = kf.predict();
    auto pred_uv = Point2f{result.at<float>(0), result.at<float>(1)};

    float width = sqrt(result.at<float>(3) * result.at<float>(2)); // width^2 = (width * height) * (width / height) = area * ratio
    float height = width / result.at<float>(2);

    return {pred_uv, Size2f{width, height}};
}


float ArmorTrack::calcSimilarity(const ArmorInfo &armor, float curr_time)
{
    float dt = curr_time - last_pred_time;
    auto new_bounding_box = (Rect2f) armor.corners_img;
    auto pred_bounding_box = predict(curr_time);

    float iou = calculateIoU(pred_bounding_box, new_bounding_box);
    float id_similarity = calcIdSimilarity(armor.armor_id);

    return iou * 0.7 + id_similarity * 0.3;
}

float ArmorTrack::calcIdSimilarity(ArmorID id)
{
    int sum = std::accumulate(id_cnt.begin(), id_cnt.end(), 2);
    return (float) (id_cnt[id] + 1) / (float) sum; // Here we artificially add one positive and one negative. To smoothen the result.
}