//
// Created by catslashbin on 23-1-29.
//

#ifndef CYGNOIDES_DECISION_CONFIG_H
#define CYGNOIDES_DECISION_CONFIG_H

#include "typing/core.h"

#include <yaml-cpp/yaml.h>

class Config {
private:

    std::string scope;
    YAML::Node node;

    static std::vector<std::string> split(const std::string &str, const std::string delimiter);

public:

    template<class T>
    T get(const std::string field) {
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
        return n.as<T>();
    }

    template<class T>
    T get(const std::string field, const T default_value) {
        T ret;
        try {
            ret = get<T>(field);
        } catch (std::exception &e) {
            warn("Failed to load {}.{}: {}, Using default value: {}", scope, field, e.what(), default_value);
            ret = default_value;
        }
        return ret;
    }

    explicit Config(const YAML::Node &node, std::string &scope) : node(node), scope(scope) {
        assert(node.IsDefined());
    }
};

class ConfigLoader {
public:

    std::string path;

    /**
     * Initialize config loader
     * @param directory Path to config directory
     */
    explicit ConfigLoader(std::string directory);

    Config load(std::string cfg_scope);

    cv::FileStorage loadOpencvConfig(std::string cfg_scope);
};

#endif //CYGNOIDES_DECISION_CONFIG_H
