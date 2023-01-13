//
// Created by catslashbin on 22-11-21.
//

/**
 * Modified from https://github.com/yasenh/sort-cpp/blob/master/src/track.cpp.
 * See https://github.com/yasenh/sort-cpp/blob/master/LICENSE for its license.
 */

#include "tracker.h"

munkres::Matrix<float> hungarianMatching(munkres::Matrix<float> mat) {

    // Debug
    std::cout << mat << std::endl;

    munkres::Munkres<float> m;
    m.solve(mat);

    // Debug
    std::cout << mat << std::endl;

    return mat;
}

void ::Tracker::update(const DetectArmorResult &detect_result) {

    const std::vector<DetectArmorInfo> &armor_detections = detect_result.armor_info;
    std::vector<DetectArmorInfo> unmatched_detections;
    std::map<int, DetectArmorInfo> matched_track2det;

    float dt = detect_result.time - last_update_time_;
    last_update_time_ = detect_result.time;

    if (detect_result.armor_info.empty()) {
        warn("Received empty detection. Skip.");
        return;
    } else {
        associate(armor_detections, dt, unmatched_detections, matched_track2det);
    }

    // Update tracks counts
    for (auto &p: armor_tracks_) {
        auto &trk = p.second;

        if (trk.missing_cnt > k_max_missing_cnt) {
            // Delete lost track
            armor_tracks_.erase(trk.tracking_id);
        } else {
            // Pre-add one missing cnt.
            trk.missing_cnt++;
        }
    }

    // Update tracks with associated detections
    for (const auto &p: matched_track2det) {
        auto &trk = armor_tracks_.at(p.first);
        trk.correct(p.second, dt);
        trk.hit_cnt++;
        trk.missing_cnt = 0;
    }

    // Create new tracks for unmatched detections
    for (const auto &det: unmatched_detections) {
        ArmorTrack track(curr_id_, det);
        armor_tracks_.at(curr_id_++) = track;
    }

}

void ::Tracker::associate(const std::vector<DetectArmorInfo> &armor_detections, float dt,
                          std::vector<DetectArmorInfo> &unmatched_detections,
                          std::map<int, DetectArmorInfo> &matched_track2det) {

    size_t n_detection = armor_detections.size(), n_tracks = armor_tracks_.size();

    // Mat[detection, track]
    munkres::Matrix<float> similarity_mat(n_detection, n_tracks);
    for (size_t i = 0; i < n_detection; ++i) {
        size_t j = 0;
        for (auto &track: armor_tracks_) {
            // Munkres originally aims to find the min cost. Multiply similarity by -1 to find max cost.
            similarity_mat(i, j) = -1.0f * track.second.calcSimilarity(armor_detections[i], dt);
            ++j;
        }
    }

    munkres::Matrix<float> association = hungarianMatching(similarity_mat);

    for (size_t i = 0; i < n_detection; ++i) {
        bool matched_flag = false;
        size_t j = 0;
        for (auto &int2track: armor_tracks_) {
            if (association(i, j) == 0) {
                if (similarity_mat(i, j) >= k_similarity_threshold) {
                    matched_track2det[int2track.first] = armor_detections[i];
                    matched_flag = true;
                }
                break; // Association is 1-to-1.
            }
            j++;
        }

        // Failed to associate.
        if (!matched_flag) {
            unmatched_detections.push_back(armor_detections[i]);
        }
    }
}

std::map<int, ArmorTrack> (::Tracker::getAllTracks)() {
    return armor_tracks_;
};