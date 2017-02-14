#ifndef STATOR_CONFIG_HPP
#define STATOR_CONFIG_HPP

#include <string>

#include <yaml-cpp/yaml.h>


namespace stator {

/**
 * Helper class for YAML configuration file parsing.
 */
class Config
{
public:
    Config(const std::string& config_path);

    template <typename ParamType>
    ParamType get_param(const std::string& param_name) {
        try {
            return config[param_name].as<ParamType>();
        } catch (YAML::InvalidNode& e) {
            std::string what_string("Missing param ");
            what_string += param_name;
            throw std::logic_error(what_string);
        }
    }

private:
    YAML::Node config;
};

} // namespace stator

#endif //STATOR_CONFIG_HPP
