//
// Created by catslashbin on 23-1-29.
//

#ifndef CYGNOIDES_DECISION_CONFIG_IMPL_H
#define CYGNOIDES_DECISION_CONFIG_IMPL_H
#include "config.h"

#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

using namespace spdlog;

class Config::ConfigImpl {
private:

    std::string path;
    std::string scope;
    YAML::Node node;

    static std::vector<std::string> split(const std::string &str, const std::string delimiter);

public:

    /**
     * Initialize config loader
     * @param path Path to config directory
     */
    explicit ConfigImpl(std::string path);

    template<class T>
    T get(std::string field) {
        auto split_str = split(field, ".");

        // Construct a new temporary node for recursion.
        // NOTE: Using `n = node` will get a reference instead of a new copy.
        YAML::Node n;
        n.reset(node);

        for (auto const &s: split_str) {
            // Same here.
            n.reset(n[s]);

            if (!n.IsDefined())
                throw std::domain_error(fmt::format("No such field: {}", s));
        }

        // debug("Loaded config param: {} = {}", field, n.as<T>());
        return n.as<T>();
    }

    template<class T>
    T get(std::string field, T default_value) {
        T ret;
        try {
            ret = get<T>(field);
        } catch (std::exception &e) {
            warn("Failed to load {}.{}: {}, Using default value: {}", scope, field, e.what(), default_value);
            ret = default_value;
        }
        return ret;
    }
};

#endif //CYGNOIDES_DECISION_CONFIG_IMPL_H
