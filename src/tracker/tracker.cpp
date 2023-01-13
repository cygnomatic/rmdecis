//
// Created by catslashbin on 22-11-21.
//

/**
 * Modified from https://github.com/yasenh/sort-cpp/blob/master/src/track.cpp.
 * See https://github.com/yasenh/sort-cpp/blob/master/LICENSE for its license.
 */

#include "tracker.h"

munkres::Matrix<float> hungarianMatching(munkres::Matrix<float> mat)
{

    // Debug
    std::cout << mat << std::endl;

    munkres::Munkres<float> m;
    m.solve(mat);

    // Debug
    std::cout << mat << std::endl;

    return mat;
}

void ::Tracker::update(DetectArmorResult detection)
{
    if (detection.armor_info.empty())
    {
        warn("Received empty detection. Skip.");
        return;
    }

    std::vector<DetectArmorInfo> &armor_detection = detection.armor_info;
    std::vector<DetectArmorInfo> unmatched_armor;

    size_t n_detection = armor_detection.size(), n_tracks = armor_tracks.size();
    float dt = detection.time - last_update_time;

    // Matching tracked armors
    unmatched_armor = match(armor_detection, dt);



}

std::vector<DetectArmorInfo> (::Tracker::match) (const std::vector<DetectArmorInfo> &armor_detection, float dt)
{
    size_t n_detection = armor_detection.size(), n_tracks = armor_tracks.size();
    std::vector<DetectArmorInfo> unmatched_armor;

    // Mat[detection, track]
    munkres::Matrix<float> similarity_mat(n_detection, n_tracks);
    for (size_t i = 0; i < n_detection; ++i)
    {
        size_t j = 0;
        for (auto &track: armor_tracks)
        {
            // Munkres originally aims to find the min cost. Multiply similarity by -1 to find max cost.
            similarity_mat(i, j) = -1.0f * track.second.calcSimilarity(armor_detection[i], dt);
            ++j;
        }
    }

    munkres::Matrix<float> association = hungarianMatching(similarity_mat);

    for (size_t i = 0; i < n_detection; ++i)
    {
        size_t j = 0;
        for (auto &track: armor_tracks)
        {
            if (association(i, j) == 0)
            {
                if (similarity_mat(i, j) >= SIMILARITY_THRESHOLD)
                {
                    track.second.correct(armor_detection[i], dt);
                    unmatched_armor.push_back(armor_detection[i]);
                }
                break; // Association is 1-to-1.
            }
        }
    }

    return unmatched_armor;
}
