#ifndef STATOR_ENGINE_HPP
#define STATOR_ENGINE_HPP

#include <map>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "reporter.hpp"
#include "serializer.hpp"


namespace po = boost::program_options;


namespace stator {

/**
 * Base class for stat collecting and reporting applications.
 *
 * Users need to perform folowing steps:
 * 1. Inherit StatsEngine.
 * 2. Override process function. Implement actual code for testing, collect
 * stats and provide it to report.
 * 3. Create StatsEngine child object and call run function.
 *
 * You can optionally override get_program_description() to provide programm
 * description in console and report output. If you need to add params to
 * programm you can override add_program_options() to add options and
 * parse_program_options() if you need to additionaly parse params after boost.
 * For one time configuration before runs you can override prepare(). If you
 * need to perform setup before and cleaning after each run you can override
 * set_up() and tear_down().
 */
class StatsEngine {
public:
    StatsEngine() = default;
    virtual ~StatsEngine() = default;

    /**
     * Parse proramm parameters, prepare everything, run code, report. Only
     * function you need to call in fully implemented child class.
     */
    void run(int argc, const char* argv[]);

protected:
    /**
     * Reporter access for inhereted classes.
     */
    Reporter& get_reporter();

    /**
     * Provide programm description for console and report.
     */
    virtual std::string get_program_description();
    /**
     * Add specific programm options.
     */
    virtual void add_program_options(po::options_description& opt_args);
    /**
     * Best place to parse specific programm options.
     */
    virtual void parse_program_options();

    /**
     * One-time preparation before actual code running.
     */
    virtual void prepare();

    /**
     * Setup before each code run.
     */
    virtual void set_up();
    /**
     * Actual code running and stats collection.
     */
    virtual void process() = 0;
    /**
     * Cleanup after each code run.
     */
    virtual void tear_down();

private:
    Reporter reporter;
    std::unique_ptr<Serializer> serializer;

    size_t run_count;

    void report();
};

} // namespace stator

#endif //STATOR_ENGINE_HPP
