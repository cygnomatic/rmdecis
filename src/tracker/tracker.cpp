//
// Created by catslashbin on 22-11-21.
//

/**
 * Modified from https://github.com/yasenh/sort-cpp/blob/master/src/track.cpp.
 * See https://github.com/yasenh/sort-cpp/blob/master/LICENSE for its license.
 */

#include "tracker.h"
#include "rmdecis/core.h"

munkres::Matrix<float> hungarianMatching(munkres::Matrix<float> mat) {
    munkres::Munkres<float> m;
    m.solve(mat);
    return mat;
}

void Tracker::update(const std::vector<ArmorInfo> &armor_detections, Time time) {
    std::vector<ArmorInfo> unmatched_detections;
    std::map<int, ArmorInfo> matched_track2det;

    if (armor_detections.empty()) {
        warn("Received empty detection. Skip.");
    } else {
        if (armor_tracks_.empty()) {
            debug("No tracking armors.");
            unmatched_detections = {armor_detections[0]};
        } else {
            associate(armor_detections, time, unmatched_detections, matched_track2det);
        }
    }

    // Update tracks with associated detections
    for (const auto &p: matched_track2det) {
        ArmorTrack &trk = armor_tracks_.at(p.first);
        trk.correct(p.second, time);
        trk.hit_cnt++;

        // Set to minus one to cancel out the extra addition of `missing_cnt` in `Update tracks lifecycle`.
        trk.missing_cnt = -1;
    }

    // Create new tracks for unmatched detections
    for (const auto &det: unmatched_detections) {
        ArmorTrack track(curr_id_, det, kf_factory);
        armor_tracks_.emplace(curr_id_++, track);
    }

    // Update tracks lifecycle
    for (auto it = armor_tracks_.begin(); it != armor_tracks_.end();) {
        // NOTE: DO NOT USE "for (auto &p: armor_tracks_)" !
        // That will cause problem when erase and make the program freeze.

        if (it->second.missing_cnt > k_max_missing_cnt) {
            if (it->second.hit_cnt > k_min_hit_cnt) {
                debug("Armor tracker {} dead. Erased.", it->second.tracking_id);
            };
            it = armor_tracks_.erase(it);
        } else {
            it->second.missing_cnt++;
            it++;
        }
    }
}

void Tracker::associate(const std::vector<ArmorInfo> &armor_detections, Time time,
                        std::vector<ArmorInfo> &unmatched_detections,
                        std::map<int, ArmorInfo> &matched_track2det) {

    size_t n_detection = armor_detections.size(), n_tracks = armor_tracks_.size();

    // Mat[detection, track]
    munkres::Matrix<float> negative_similarity_mat(n_detection, n_tracks);
    for (size_t i = 0; i < n_detection; ++i) {
        size_t j = 0;
        for (auto &track: armor_tracks_) {
            // Munkres originally aims to find the min cost. Multiply similarity by -1 to find max cost.
            negative_similarity_mat(i, j) = -1.0f * track.second.calcSimilarity(armor_detections[i], time);
            ++j;
        }
    }

    munkres::Matrix<float> association = hungarianMatching(negative_similarity_mat);

    for (size_t i = 0; i < n_detection; ++i) {
        bool matched_flag = false;
        size_t j = 0;
        for (auto &int2track: armor_tracks_) {
            if (association(i, j) == 0) {
                if (-negative_similarity_mat(i, j) >= k_similarity_threshold) {
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

std::map<int, ArmorTrack> Tracker::getTracks(bool include_probationary) {
    if (include_probationary)
        return armor_tracks_;

    std::map<int, ArmorTrack> tracks;
    for (auto &p: armor_tracks_) {
        if (p.second.hit_cnt > k_min_hit_cnt)
            tracks.insert(p);
    }
    return tracks;
}

Tracker::Tracker(Config &cfg) : kf_factory(cfg) {
    k_similarity_threshold = cfg.get<float>("tracker.similarityThreshold", 0.7);
    k_max_missing_cnt = cfg.get<int>("tracker.maxMissingCount", 2);
    k_min_hit_cnt = cfg.get<int>("tracker.minHitCount", 4);
}
