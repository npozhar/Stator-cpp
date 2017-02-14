#ifndef STATOR_SERIALIZER_HPP
#define STATOR_SERIALIZER_HPP

#include <string>
#include <map>

#include "reporter.hpp"


namespace stator {

/**
 * Base serializer class. It used by StatsEngine
 */
class Serializer {
public:
    /**
     * Performs actual serialization. Must write result to standard output.
     * Typically output consists of three parts: description, run params and
     * collected stats.
     */
    virtual void serialize(const Reporter& reporter) = 0;

protected:
    /*
     * For child access to private reporter methods
     */
    static const std::string& get_description(const Reporter& reporter);
    static const std::map<std::string, Reporter::Param>
    get_params(Reporter::ParamType param_type, const Reporter& reporter);
};

/**
 * Console serialzier
 */
class ConsoleSerializer : public Serializer {
public:
    void serialize(const Reporter& reporter) override;
};

/**
 * JSON serializer
 */
class JsonSerializer : public Serializer {
public:
    void serialize(const Reporter& reporter) override;
};

} // namespace stator

#endif //STATOR_SERIALIZER_HPP
