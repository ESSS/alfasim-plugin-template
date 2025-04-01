#include "TemplateModel.hpp"

TemplateModel::TemplateModel(
    double _quantity_value,
    EnumOption _enum_value,
    std::string _string_value,
    std::string _file_content_value,
    std::vector<double> _table_value_pressure,
    std::vector<double> _table_value_temperature,
    int _reference_tracer_value,
    double _reference_internal_quantity_value,
    std::vector<int> _multiple_reference_tracer_values,
    std::vector<double> _multiple_reference_internal_values)
    : quantity_value(_quantity_value)
    , enum_value(_enum_value)
    , string_value(_string_value)
    , file_content_value(_file_content_value)
    , table_value_pressure(_table_value_pressure)
    , table_value_temperature(_table_value_temperature)
    , reference_tracer_value(_reference_tracer_value)
    , reference_internal_quantity_value(_reference_internal_quantity_value)
    , multiple_reference_tracer_values(_multiple_reference_tracer_values)
    , multiple_reference_internal_values(_multiple_reference_internal_values) { };
