//
// Created by catslashbin on 23-2-2.
//

#include "config.h"
#include "rmdecis_impl/config_impl.h"

Config::Config(std::string path) : impl(new ConfigImpl(path)) {}

Config::~Config() = default;

template<class T>
T Config::get(std::string field, T default_value) {
    return impl->get<T>(field, default_value);
}

template<class T>
T Config::get(std::string field) {
    return impl->get<T>(field);
}

template std::string Config::get(std::string field, std::string default_value);
template int Config::get(std::string field, int default_value);
template float Config::get(std::string field, float default_value);
template double Config::get(std::string field, double default_value);

template std::string Config::get(std::string field);
template int Config::get(std::string field);
template float Config::get(std::string field);
template double Config::get(std::string field);