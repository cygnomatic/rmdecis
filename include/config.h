//
// Created by catslashbin on 23-2-2.
//

#ifndef CYGNOIDES_DECISION_CONFIG_H
#define CYGNOIDES_DECISION_CONFIG_H

#include "typing.h"

class Config {
public:

    /**
     * Initialize config loader
     * @param path Path to config directory
     */
    explicit Config(std::string path);

    /**
     * Get item from config.
     * @tparam T Type
     * @param field Config field
     * @return The corresponding config item, throw runtime_error if not exist.
     */
    template<class T>
    T get(std::string field);

    /**
     * Get item form config with default value.
     * @tparam T Type
     * @param field Config field
     * @param default_value Default value
     * @return The corresponding config item if the item exist. Otherwise return the default value .
     */
    template<class T>
    T get(std::string field, T default_value);

private:
    class ConfigImpl;
    std::unique_ptr<ConfigImpl> impl;
};

#endif //CYGNOIDES_DECISION_CONFIG_H
