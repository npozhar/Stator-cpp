
#include "config.hpp"


stator::Config::Config(const std::string& config_path)
        : config(YAML::LoadFile(config_path))
{
     if (!config.IsMap()) {
        throw std::logic_error("Config file couldn't be loaded");
    }
}
