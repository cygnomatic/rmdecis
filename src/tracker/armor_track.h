//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACK_H
#define CYGNOIDES_DECISION_ARMOR_TRACK_H

#include <numeric>

#include "rmdecis/core.h"
#include "utils/cv_utils.h"
#include "tracker/track_kalman.h"
#include "typing/typing_internal.h"


class ArmorTrack {
public:
    int tracking_id = -1;
    int missing_cnt = 0, hit_cnt = 0;

    cv::RotatedRect last_bbox_;
    TrackKalman track_kalman;

    explicit ArmorTrack(int tracking_id, const ArmorInfo &detection, int frame_seq, Config &cfg);

    void init(const ArmorInfo &detection);

    void correct(const ArmorInfo &detection, int frame_seq);

    TrackArmorInfo predict(int frame_seq);

    float calcSimilarity(const ArmorInfo &detection, int frame_seq);

private:

    std::array<int, MAX_FACILITY_ID> id_cnt{};

    float calcIdSimilarity(FacilityID id);

    float calcCenterDistSimilarity(const cv::Point3f &center);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACK_H
