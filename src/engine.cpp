
#include <iostream>

#include "engine.hpp"


using stator::Reporter;


void stator::StatsEngine::run(int argc, const char* argv[])
{
    std::string serilalizer_name;

    po::positional_options_description pos_args;
    po::options_description generic_opt_args("Stats app");
    generic_opt_args.add_options()
            ("help,h", "Display this help message")
            ("serializer,s",
             po::value<std::string>(&serilalizer_name)->default_value("plain"),
             "Serilizer type (plain or json)")
            ("run_count,r",
             po::value<size_t>(&run_count)->default_value(1),
             "Number of runs for stats collection")
            ;

    po::options_description opt_args;

    opt_args.add(generic_opt_args);

    add_program_options(opt_args);

    po::variables_map vm;
    try {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(opt_args).positional(pos_args).run();
        po::store(parsed, vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << get_program_description() << std::endl
                      << opt_args << std::endl;
            exit(0);
        }

        if (!serilalizer_name.compare("plain")) {
            serializer = std::unique_ptr<ConsoleSerializer>(new ConsoleSerializer);
        }
        else if (!serilalizer_name.compare("json")) {
            serializer = std::unique_ptr<JsonSerializer>(new JsonSerializer);
        }
        else {
            throw std::logic_error("Wrong serializer type provided");
        }

        this->get_reporter().template add_param<size_t>(
            Reporter::ParamType::Input, "run_count", "Run count", run_count);

        parse_program_options();

    } catch (std::exception& exc) {
        std::cerr << "Error parsing arguments: " << exc.what() << std::endl;
        std::cout << get_program_description() << std::endl
                  << opt_args << std::endl;
        exit(1);
    }

    try {
        prepare();

        for (size_t i = 0; i < run_count; ++i) {
            set_up();
            process();
            tear_down();
        }
    } catch (std::exception& exc) {
        this->get_reporter().template add_param<std::string>(
            Reporter::ParamType::Stat, "error", exc.what());
        report();
        exit(1);
    }

    report();
}


Reporter& stator::StatsEngine::get_reporter()
{
    return reporter;
}


std::string stator::StatsEngine::get_program_description()
{
    return "Base stats app";
}


void stator::StatsEngine::add_program_options(
    po::options_description& /* opt_args */)
{ }


void stator::StatsEngine::parse_program_options()
{ }


void stator::StatsEngine::prepare()
{ }


void stator::StatsEngine::set_up()
{ }


void stator::StatsEngine::tear_down()
{ }


void stator::StatsEngine::report()
{
    serializer->serialize(reporter);
}
