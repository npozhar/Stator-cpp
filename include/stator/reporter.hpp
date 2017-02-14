#ifndef STATOR_REPORTER_HPP
#define STATOR_REPORTER_HPP

#include <map>
#include <vector>
#include <string>

#include <boost/variant.hpp>

namespace stator {

/**
 * Reporter collects all stats during run and provides final results.
 *
 * For adding stats use add_param(). If add_param() called multiple times,
 * then all values saved. For detailed description see add_param() docs.
 */
class Reporter {
    /** For description and collected params access by serializer */
    friend class Serializer;

    using ValueType = boost::variant<int, size_t, double, std::string>;

public:
    using FormatterType = std::function<std::string(const ValueType& value)>;

    /**
     * Param data structure. Holds metadata and all provided values.
     */
    struct Param {
        std::string name;
        std::string verbose_name;
        std::vector<ValueType> values;
        FormatterType formatter;

        Param() = delete;
        template <typename ValueT>
        Param(const std::string& n, const std::string& v_n,
              const ValueT& v, const FormatterType& f = nullptr)
            : name(n), verbose_name(v_n), values({ { v } }), formatter(f)
        { }

        template <typename ValueT>
        void add_value(const ValueT& value)
        {
            values.push_back(value);
        }

        struct DoubleVisitor : public boost::static_visitor<double>
        {
            template <typename ValueT>
            double operator()(ValueT value) const { return value; }
            double operator()(const std::string& /* value */) const {
                throw std::logic_error("Can't convert string to double");
            }
        };

        ValueType get_value() const
        {
            if (values.empty()) {
                throw std::logic_error("No value stored");
            } else if (values.size() == 1) {
                return values[0];
            } else {
                if (values[0].type() == typeid(std::string)) {
                    return values.back();
                } else {
                    DoubleVisitor doubleVisitor;
                    auto sum_values = boost::apply_visitor(doubleVisitor, values[0]);
                    for (size_t i = 1; i < values.size(); ++i) {
                        sum_values += boost::apply_visitor(doubleVisitor, values[i]);
                    }
                    return sum_values / values.size();
                }
            }
        }
    };

    enum class ParamType {
        Input,
        Stat
    };

    Reporter() = default;

    /**
     * Programm description for report.
     */
    void set_description(const std::string& description);

    /**
     * Save param for future reporting. If same param_type and name pair
     * provided multiple times, then only first call verbose_name and formatter
     * used and all values saved. Final value calculated by following rules:
     * - for numeric types final value is average
     * - for non-numeric types final value is last
     */
    template <typename ValueT>
    void add_param(
        ParamType param_type, const std::string& name,
        const std::string& verbose_name, const ValueT& value,
        const FormatterType& formatter = nullptr)
    {
        try {
            params[param_type].at(name).add_value(value);
        } catch (std::out_of_range&) {
            params[param_type].emplace(
                std::make_pair(name, Param(name, verbose_name, value, formatter)));
        }
    }

    /**
     * Shortcut without verbose name (name used for verbose name).
     */
    template <typename ValueT>
    void add_param(
        ParamType param_type, const std::string& name,
        const ValueT& value, const FormatterType& formatter = nullptr)
    {
        add_param<ValueT>(param_type, name, name, value, formatter);
    }

private:
    std::string description;

    std::map<ParamType, std::map<std::string, Param>> params;
};

} // namespace stator

#endif //STATOR_REPORTER_HPP
