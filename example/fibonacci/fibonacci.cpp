
#include <chrono>

#include "stator/engine.hpp"
#include "stator/config.hpp"


class FibonacciStatsEngine : public stator::StatsEngine
{
private:
    std::string config_path;
    std::unique_ptr<stator::Config> config;
    int fibonachi_input;

    std::string get_program_description() override
    {
        return "Example program build with Stator micro-framework";
    }

    /*
     * Demonstrates adding your own program options.
     */
    void add_program_options(po::options_description& opt_args) override
    {
        po::options_description ec_opt_args("Fibonacci Stator");
        ec_opt_args.add_options()
            ("config,c",
             po::value<std::string>(&config_path)->required(),
             "Fibonacci config path")
            ;

        opt_args.add(ec_opt_args);
    }

    /*
     * When you add own program options you must handle them in
     * parse_program_options.
     */
    void parse_program_options() override
    {
        config = std::unique_ptr<stator::Config>(new stator::Config(config_path));
    }

    /*
     * One-time configuration for all runs.
     */
    void prepare() override
    {
        // It is right place to provide program description for reporter.
        this->get_reporter().set_description(get_program_description());

        fibonachi_input = config->get_param<int>("fibonacci_input");

        // All program options that you want to save to Stator output must
        // be explicitly added to reporter.
        this->get_reporter().template add_param<int>(
            stator::Reporter::ParamType::Input,
            "fibonacci_input", "Fibonacci input", fibonachi_input);
    }

    /*
     * Actual fibonacci implementation.
     */
    int fibonacci(int x)
    {
        if (x == 0) {
            return 0;
        }

        if (x == 1) {
            return 1;
        }

        return fibonacci(x - 1) + fibonacci(x - 2);
    }

    /*
     * One iteration of observed code implementation. Here you must collect
     * all stats on add it to reporter.
     */
    void process() override
    {
        // In process function collect stats ...
        auto start_time = std::chrono::high_resolution_clock::now();
        auto fibonachi_output = fibonacci(fibonachi_input);
        auto nanosec_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - start_time).count();
        auto seconds_duration = static_cast<double>(nanosec_duration) * 1e-9;

        // ... and add it to reporter. If process run multiple times
        // then reporter accumulates all numeric stats and report average. ...
        this->get_reporter().template add_param<int>(
            stator::Reporter::ParamType::Stat,
            "fibonacci_output", "Fibonacci output", fibonachi_output);
        this->get_reporter().template add_param<double>(
            stator::Reporter::ParamType::Stat,
            "fibonacci_duration", "Fibonacci duration", seconds_duration);
        // ... If string stats used, then only last value will be reported.
        this->get_reporter().template add_param<std::string>(
            stator::Reporter::ParamType::Stat,
            "status", "Status", "Success");
    }
};


int main(int argc, const char* argv[])
{
    // In main function create StatsEngine derived object and call run.
    FibonacciStatsEngine stats_engine;
    stats_engine.run(argc, argv);

    return 0;
}

