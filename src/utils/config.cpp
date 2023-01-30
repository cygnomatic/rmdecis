//
// Created by catslashbin on 23-1-29.
//

#include <filesystem>
#include <string>
#include <opencv2/core.hpp>

#include "utils/config.h"

using namespace std;
namespace fs = filesystem;

ConfigLoader::ConfigLoader(string directory) {

    if (!fs::is_directory(directory))
        throw runtime_error(fmt::format("Config directory not found: {}.", directory));

    info("Config directory: {}", directory);
    path = directory;
}

Config ConfigLoader::load(string cfg_scope) {
    debug("Loading config scope: {}", cfg_scope);
    return Config(YAML::LoadFile(fmt::format("{}/{}.yml", path, cfg_scope)), cfg_scope);
}

cv::FileStorage ConfigLoader::loadOpencvConfig(string cfg_scope) {
    debug("Loading config scope: {}", cfg_scope);

    string coeffs_path = fmt::format("{}/{}.yml", path, cfg_scope);
    cv::FileStorage fs(coeffs_path, cv::FileStorage::READ);

    if (!fs.isOpened()) {
        throw std::runtime_error("Can not open file: " + coeffs_path);
    }

    return fs;
}

vector<string> Config::split(const string &str, const string delimiter) {

    vector<string> split_str;
    size_t last = 0;
    size_t next = 0;

    while ((next = str.find(delimiter, last)) != string::npos) {
        split_str.push_back(str.substr(last, next - last));
        last = next + 1;
    }

    split_str.push_back(str.substr(last));
    return split_str;
}
