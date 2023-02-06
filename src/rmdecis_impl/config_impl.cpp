//
// Created by catslashbin on 23-1-29.
//

#include <filesystem>
#include <string>
#include <spdlog/spdlog.h>

#include "config.h"
#include "config_impl.h"

using namespace std;

vector<string> Config::ConfigImpl::split(const string &str, const string delimiter) {
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

Config::ConfigImpl::ConfigImpl(std::string path) : path(path) {
    try {
        path = filesystem::absolute(path).lexically_normal().string();
    } catch (exception &e) {
        error("Failed to load config form: {}", path);
        throw runtime_error(fmt::format("Failed to load config: {}", e.what()));
    }
    info("Loading config form: {}", path);

    try {
        node = YAML::LoadFile(path);
    } catch (exception &e) {
        error("Failed to load config from: {}", path);
        throw runtime_error(fmt::format("Failed to load config: {}", e.what()));
    }

    if (!node.IsDefined()) {
        error("Invalid config: {}", path);
        throw std::runtime_error(fmt::format("Invalid config path: {}", path));
    }
}
