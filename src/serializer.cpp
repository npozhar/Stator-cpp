
#include "serializer.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


using stator::Reporter;


const std::string& stator::Serializer::get_description(const Reporter& reporter)
{
    return reporter.description;
}


const std::map<std::string, Reporter::Param> stator::Serializer::get_params(
    Reporter::ParamType param_type,
    const Reporter& reporter)
{
    return reporter.params.at(param_type);
}


void stator::ConsoleSerializer::serialize(const Reporter& reporter)
{
    std::cout << get_description(reporter) << std::endl;

    auto print = [](const Reporter::Param& param){
        std::cout << param.verbose_name << ": "
                  << (param.formatter ? param.formatter(param.get_value()) : param.get_value())
                  << std::endl;
    };

    std::cout << std::endl << "-*- Run params -*-" << std::endl;
    try {
        for (const auto& param: get_params(Reporter::ParamType::Input, reporter)) {
            print(param.second);
        }
    } catch (std::out_of_range&) {
        std::cout << "No run params specified" << std::endl;
    }


    std::cout << std::endl << "-*- Run stats -*-" << std::endl;
    try {
        for (const auto& param: get_params(Reporter::ParamType::Stat, reporter)) {
            print(param.second);
        }
    } catch (std::out_of_range&) {
        std::cout << "No run stats collected" << std::endl;
    }

}


void stator::JsonSerializer::serialize(const Reporter& reporter)
{
    boost::property_tree::ptree result;

    result.put("description", get_description(reporter));

    auto add_param_to_node = [](boost::property_tree::ptree& node,
                                const Reporter::Param& param){
        node.put(
            param.name,
            param.formatter ? param.formatter(param.get_value()) : param.get_value());
    };

    boost::property_tree::ptree input_params;
    try {
        for (const auto& param: get_params(Reporter::ParamType::Input, reporter)) {
            add_param_to_node(input_params, param.second);
        }
    } catch (std::out_of_range&) { }
    result.add_child("input_params", input_params);

    boost::property_tree::ptree stats;
    try {
        for (const auto& param: get_params(Reporter::ParamType::Stat, reporter)) {
            add_param_to_node(stats, param.second);
        }
    } catch (std::out_of_range&) { }
    result.add_child("stats", stats);

    boost::property_tree::write_json(std::cout, result);
}
