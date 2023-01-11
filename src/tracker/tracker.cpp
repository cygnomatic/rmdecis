//
// Created by catslashbin on 22-11-21.
//

/**
 * Modified from https://github.com/yasenh/sort-cpp/blob/master/src/track.cpp.
 * See https://github.com/yasenh/sort-cpp/blob/master/LICENSE for its license.
 */

#include "tracker.h"

void ::Tracker::update(DetectArmorResult detection)
{
    if (detection.armor_info.empty())
    {
        warn("Received empty detection. Skip.");
        return;
    }

    std::vector<DetectArmorInfo> &armor_detection = detection.armor_info;
    size_t n_detection = armor_detection.size(), n_tracks = armor_tracks.size();
    float dt = detection.time - last_update_time;

    // Mat[detection, track]
    munkres::Matrix<float> similarity_mat(n_detection, n_tracks);
    for (size_t i = 0; i < n_detection; ++i)
    {
        size_t j = 0;
        for (auto& track: armor_tracks)
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
        for (auto& track: armor_tracks)
        {
            if (association(i, j) == 0){
                if (similarity_mat(i, j) >= SIMILARITY_THRESHOLD)
                    track.second.correct(armor_detection[i], dt);
                break; // Association is 1-to-1.
            }
        }
    }

}

munkres::Matrix<float> (::Tracker::hungarianMatching)(munkres::Matrix<float> mat)
{
    // Debug
    std::cout << mat << std::endl;

    munkres_f.solve(mat);

    // Debug
    std::cout << mat << std::endl;

    return mat;
}