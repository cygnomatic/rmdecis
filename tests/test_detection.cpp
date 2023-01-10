//
// Created by catslashbin on 23-1-8.
//

#include "../utils_contrib/simple_video_player.h"
#include "../src/contrib/detection/get_zhuangjiaban.h"
#include "../src/utils/cv_utils.h"
#include <yaml-cpp/yaml.h>

void emitYaml(YAML::Emitter& out, const vector<ArmorPredResult>& pred_results, int seq)
{
    out << YAML::BeginDoc;
    {
        out << YAML::BeginMap;
        {
            out << YAML::Key << "header" << YAML::Value << YAML::BeginMap;
            {
                out << YAML::Key << "seq" << YAML::Value << seq;
                out << YAML::Key << "stamp" << YAML::Value << YAML::BeginMap;
                {
                    out << YAML::Key << "secs" << YAML::Value << seq;
                    out << YAML::Key << "nsecs" << YAML::Value << 0;
                }
                out << YAML::EndMap;
                out << YAML::Key << "frame_id" << YAML::Value << "";
            }
            out << YAML::EndMap;

            out << YAML::Key << "data" << YAML::Value << YAML::BeginSeq;
            {
                for (auto &p: pred_results)
                {
                    out << YAML::BeginMap;
                    {
                        out << YAML::Key << "armor_type" << YAML::Value << 3;
                        out << YAML::Key << "confidence" << YAML::Value << p.confidence;
                        out << YAML::Key << "rt_x" << YAML::Value << p.corners_img.tr.x;
                        out << YAML::Key << "rt_y" << YAML::Value << p.corners_img.tr.y;
                        out << YAML::Key << "lt_x" << YAML::Value << p.corners_img.tl.x;
                        out << YAML::Key << "lt_y" << YAML::Value << p.corners_img.tl.y;
                        out << YAML::Key << "ld_x" << YAML::Value << p.corners_img.dl.x;
                        out << YAML::Key << "ld_y" << YAML::Value << p.corners_img.dl.y;
                        out << YAML::Key << "rd_x" << YAML::Value << p.corners_img.dr.x;
                        out << YAML::Key << "rd_y" << YAML::Value << p.corners_img.dr.y;
                    }
                    out << YAML::EndMap;
                }
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
    }
    out << YAML::EndDoc;

    assert(out.good());
    // std::cout << out.c_str() << endl;
}

int main()
{
    // SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    // player.setPlaybackSpeed(5);
    // player.setRecordSpeed(0.1);

    VideoCapture cap("../../data/vision_out/video_input.avi");
    std::ofstream file("vision_result.yaml");
    Mat frame;
    YAML::Emitter out;
    int frame_pos = 0, all_frame = cap.get(CAP_PROP_FRAME_COUNT);

    info("Start process {} frame.", all_frame);
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        auto vision_out = detect(frame, blue);

        for (auto armor_info: vision_out)
        {
            drawArmorCorners(frame, armor_info.corners_img, {0, 0, 255});
        }

        emitYaml(out, vision_out, frame_pos++);

        if (frame_pos % 50 == 0)
            info("Processing {}/{} frame ...", frame_pos, all_frame);
    }
    file << out.c_str();
    info("Process finished.");
}