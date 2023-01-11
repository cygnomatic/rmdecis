//
// Created by catslashbin on 22-11-21.
//

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

    // Mat[detection, track]
    munkres::Matrix<float> munkres_matrix(n_detection, n_tracks);
    for (size_t i = 0; i < n_detection; ++i)
    {
        size_t j = 0;
        for (auto track: armor_tracks)
        {
            // Munkres originally aims to find the min cost. Multiply similarity by -1 to find max cost.
            munkres_matrix(i, j) = -1.0f * track.second.calcSimilarity(armor_detection[i],
                                                                       detection.time - last_update_time);
            ++j;
        }
    }

    // Debug
    std::cout << munkres_matrix << std::endl;

    munkres_f.solve(munkres_matrix);




}

void ::Tracker::hungarianMatching(const std::vector<std::vector<float>> &iou_matrix,
                                  size_t nrows, size_t ncols,
                                  std::vector<std::vector<float>> &association)
{

    munkres::Matrix<float> matrix(nrows, ncols);
    // Initialize matrix with IOU values
    for (size_t i = 0; i < nrows; i++)
    {
        for (size_t j = 0; j < ncols; j++)
        {
            // Multiply by -1 to find max cost
            if (iou_matrix[i][j] != 0)
            {
                matrix(i, j) = -iou_matrix[i][j];
            } else
            {
                matrix(i, j) = 1.0f;
            }
        }
    }

    //    // Display begin matrix state.
    //    for (size_t row = 0 ; row < nrows ; row++) {
    //        for (size_t col = 0 ; col < ncols ; col++) {
    //            std::cout.width(10);
    //            std::cout << matrix(row,col) << ",";
    //        }
    //        std::cout << std::endl;
    //    }
    //    std::cout << std::endl;


    // Apply Kuhn-Munkres algorithm to matrix.
    munkres::Munkres<float> m;
    m.solve(matrix);

    //    // Display solved matrix.
    //    for (size_t row = 0 ; row < nrows ; row++) {
    //        for (size_t col = 0 ; col < ncols ; col++) {
    //            std::cout.width(2);
    //            std::cout << matrix(row,col) << ",";
    //        }
    //        std::cout << std::endl;
    //    }
    //    std::cout << std::endl;

    for (size_t i = 0; i < nrows; i++)
    {
        for (size_t j = 0; j < ncols; j++)
        {
            association[i][j] = matrix(i, j);
        }
    }
}
