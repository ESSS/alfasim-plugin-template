#ifndef _HPP_TEMPLATE_MODEL
#define _HPP_TEMPLATE_MODEL
#include <string>
#include <vector>

enum class EnumOption
{
    value_a = 0,
    value_b = 1,
};

class TemplateModel
{
public:
    TemplateModel(
        double _quantity_value,
        EnumOption _enum_value,
        std::string _string_value,
        std::string _file_content_value,
        std::vector<double> _table_value_pressure,
        std::vector<double> _table_value_temperature,
        int _reference_tracer_value,
        double _reference_internal_quantity_value,
        std::vector<int> _multiple_reference_tracer_values,
        std::vector<double> _multiple_reference_internal_values);

private:
    double quantity_value;
    EnumOption enum_value;
    std::string string_value;
    std::string file_content_value;
    std::vector<double> table_value_pressure;
    std::vector<double> table_value_temperature;
    int reference_tracer_value;
    double reference_internal_quantity_value;
    std::vector<int> multiple_reference_tracer_values;
    std::vector<double> multiple_reference_internal_values;
};

#endif
