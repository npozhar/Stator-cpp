# Stator-cpp

Stats collection and reporting micro-framework. It provides API and barebone for applications than run some code, collect stats and report.

## Requremets

- [Boost](http://www.boost.org/) for programm options parsing and JSON-serialization
- [YAML-cpp](https://github.com/jbeder/yaml-cpp/) for YAML configuration parsing

## Getting started

For inspiration see fibonacci in [example directory](https://github.com/npozhar/Stator-cpp/tree/master/example). Documentation in code describes main steps to your own stats collecting app.

## Main concept

Stator consists of `StatsEngine`, `Reporter`, `Serializer` and helpers.

### `StatsEngine`

Barebone of stats collecting app. It provides command line options parsing and template method `run()`. For you own app you must inherit `StatsEngine` and override at least `process()` method. See docs in [engine.hpp](https://github.com/npozhar/Stator-cpp/tree/master/include/stator/engine.hpp) for detailed description.

### `Reporter`

Сollector of stats and final result provider. Provides API for stats saving during run. For detailed description see [reporter.hpp](https://github.com/npozhar/Stator-cpp/tree/master/include/stator/reporter.hpp).

### `Serializer`

Serializer 😊 Now you have two options: plain text or JSON. All collected by `Reporter` stats serialized by `Serializer` to standard output. If you want to save stats to file, redirect output 😉

### Helpers

For now we have only YAML-config parser (see [config.hpp](https://github.com/npozhar/Stator-cpp/tree/master/include/stator/config.hpp)). Some execution time measuring staff may be added in future.
