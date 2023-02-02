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

    template<class T>
    T get(std::string field);

    template<class T>
    T get(std::string field, T default_value);

private:
    class ConfigImpl;
    std::unique_ptr<ConfigImpl> impl;
};

#endif //CYGNOIDES_DECISION_CONFIG_H
