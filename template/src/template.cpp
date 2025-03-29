#include "TemplateModel.hpp"
#include "hook_specs.h"
#include <alfasim_sdk_api/alfasim_sdk.h>
#include <string>
#include <vector>

ALFAsimSDK_API alfasim_sdk_api;

HOOK_INITIALIZE(ctx)
{
    const char* plugin_id = get_plugin_id();
    int error_code = 0;  // 0 means no error

    // Loading ALFAsim-SDK API
    error_code = alfasim_sdk_open(&alfasim_sdk_api);
    if (error_code != 0) {
        return error_code;
    }

    // We can present informations and warnings to the user, which will be printed
    // on log_calc.txt file.
    error_code = alfasim_sdk_api.log_information_message(
        ctx, get_plugin_id(), "Successfully opened alfasim_sdk_api");
    if (error_code != 0) {
        return error_code;
    }

    // Reading info from the user interface

    // Since we are using a container_model to select one of the data_model of type
    // TemplateModel, we need to define a string with a prefix to add when calling the
    // get_plugin_input_data_* functions
    std::string prefix("ModelSelector.selected_model->");

    // Quantity Type
    double quantity_value;
    error_code = alfasim_sdk_api.get_plugin_input_data_quantity(
        ctx, &quantity_value, plugin_id, (prefix + "quantity").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.log_information_message(
        ctx,
        get_plugin_id(),
        std::string("Got quantity:" + std::to_string(quantity_value)).c_str());
    if (error_code != 0) {
        return error_code;
    }

    // Enum Type: notice that we receive a int which might be converted to a C++ enum here
    int enum_value;
    error_code = alfasim_sdk_api.get_plugin_input_data_enum(
        ctx, &enum_value, plugin_id, (prefix + "enum").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.log_information_message(
        ctx, get_plugin_id(), std::string("Got enum:" + std::to_string(enum_value)).c_str());
    if (error_code != 0) {
        return error_code;
    }

    // String Type: notice that we need to get the size and the data to create the string
    int string_size;
    char string_data[100];  // For simplicity we pre-allocate memory, but we can use malloc as well.
    error_code = alfasim_sdk_api.get_plugin_input_data_string_size(
        ctx, &string_size, plugin_id, (prefix + "string").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.get_plugin_input_data_string(
        ctx, string_data, plugin_id, (prefix + "string").c_str(), string_size);
    if (error_code != 0) {
        return error_code;
    }
    std::string string_value = { string_data, std::string::size_type(string_size) };
    error_code = alfasim_sdk_api.log_information_message(
        ctx, get_plugin_id(), std::string("Got string:" + string_value).c_str());
    if (error_code != 0) {
        return error_code;
    }

    // FileContent Type: similar to string, but we will use dynamic allocation for this one
    int file_content_size;
    error_code = alfasim_sdk_api.get_plugin_input_data_file_content_size(
        ctx, &file_content_size, plugin_id, (prefix + "file_content").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.log_information_message(
        ctx,
        get_plugin_id(),
        std::string("Got file_content size:" + std::to_string(file_content_size)).c_str());
    if (error_code != 0) {
        return error_code;
    }

    char* file_content_data = new char[file_content_size + 1];
    file_content_data = (char*) malloc(sizeof(char) * file_content_size);
    error_code = alfasim_sdk_api.get_plugin_input_data_file_content(
        ctx, file_content_data, plugin_id, (prefix + "file_content").c_str(), file_content_size);
    if (error_code != 0) {
        return error_code;
    }
    std::string file_content_value = { file_content_data,
                                       std::string::size_type(file_content_size) };
    error_code = alfasim_sdk_api.log_information_message(
        ctx, get_plugin_id(), std::string("Got file content:" + file_content_value).c_str());
    if (error_code != 0) {
        return error_code;
    }
    delete file_content_data;

    // Table Type
    double* table_value_pressure_ptr;
    double* table_value_temperature_ptr;
    int table_size;  // Same for pressure and temperature
    error_code = alfasim_sdk_api.get_plugin_input_data_table_quantity(
        ctx,
        &table_value_pressure_ptr,
        &table_size,
        "pressure",
        plugin_id,
        (prefix + "table").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.get_plugin_input_data_table_quantity(
        ctx,
        &table_value_temperature_ptr,
        &table_size,
        "temperature",
        plugin_id,
        (prefix + "table").c_str());
    if (error_code != 0) {
        return error_code;
    }
    std::vector<double> table_value_pressure(
        table_value_pressure_ptr, table_value_pressure_ptr + table_size);
    std::vector<double> table_value_temperature(
        table_value_temperature_ptr, table_value_temperature_ptr + table_size);
    error_code = alfasim_sdk_api.log_information_message(
        ctx,
        get_plugin_id(),
        std::string("Got table value: " + std::to_string(table_value_pressure[0])).c_str());
    if (error_code != 0) {
        return error_code;
    }

    // Tracer Reference Type: get the tracer selected by the user and afterwards use the
    // get_tracer_id() function to get the id used internally by that tracer on alfasim.
    void* reference_tracer_ptr;
    error_code = alfasim_sdk_api.get_plugin_input_data_reference(
        ctx, &reference_tracer_ptr, plugin_id, (prefix + "reference_tracer").c_str());
    if (error_code != 0) {
        return error_code;
    }
    int reference_tracer_value = -1;
    error_code = alfasim_sdk_api.get_tracer_id(ctx, &reference_tracer_value, reference_tracer_ptr);
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.log_information_message(
        ctx,
        get_plugin_id(),
        std::string("Got reference tracer id:" + std::to_string(reference_tracer_value)).c_str());
    if (error_code != 0) {
        return error_code;
    }

    // Internal Reference Type: instead of a call to get_plugin_input_data_reference, we will
    // directly call the corresponding function to the data type we want, remembering to use the
    // arrow symbol (->)
    double reference_internal_quantity_value;
    error_code = alfasim_sdk_api.get_plugin_input_data_quantity(
        ctx,
        &reference_internal_quantity_value,
        plugin_id,
        (prefix + "reference_internal->quantity").c_str());
    if (error_code != 0) {
        return error_code;
    }
    error_code = alfasim_sdk_api.log_information_message(
        ctx,
        get_plugin_id(),
        std::string(
            "Got quantity (internal reference):"
            + std::to_string(reference_internal_quantity_value))
            .c_str());
    if (error_code != 0) {
        return error_code;
    }

    // Tracer Multiple Reference
    int tracer_indexes_size;
    error_code = alfasim_sdk_api.get_plugin_input_data_multiplereference_selected_size(
        ctx, &tracer_indexes_size, get_plugin_id(), (prefix + "multiple_reference_tracer").c_str());

    if (error_code != 0) {
        return error_code;
    }
    std::vector<int> multiple_reference_tracer_values(tracer_indexes_size, -1);
    for (int i = 0; i < tracer_indexes_size; ++i) {
        reference_tracer_ptr = nullptr;
        auto reference_str = prefix + "multiple_reference_tracer[" + std::to_string(i) + "]";
        error_code = alfasim_sdk_api.get_plugin_input_data_reference(
            ctx, &reference_tracer_ptr, get_plugin_id(), reference_str.c_str());

        if (error_code != 0) {
            return error_code;
        }
        int multiple_reference_tracer_value = -1;
        error_code = alfasim_sdk_api.get_tracer_id(
            ctx, &multiple_reference_tracer_value, reference_tracer_ptr);
        if (error_code != 0) {
            return error_code;
        }
        multiple_reference_tracer_values[i] = multiple_reference_tracer_value;
        error_code = alfasim_sdk_api.log_information_message(
            ctx,
            get_plugin_id(),
            std::string(
                "Got multiple reference tracer id:"
                + std::to_string(multiple_reference_tracer_value))
                .c_str());
        if (error_code != 0) {
            return error_code;
        }
    }

    // Internal Multiple Reference
    int internal_indexes_size;
    error_code = alfasim_sdk_api.get_plugin_input_data_multiplereference_selected_size(
        ctx,
        &internal_indexes_size,
        get_plugin_id(),
        (prefix + "multiple_reference_internal").c_str());
    if (error_code != 0) {
        return error_code;
    }

    std::vector<double> multiple_reference_internal_values(internal_indexes_size, -1.0);
    for (int i = 0; i < internal_indexes_size; ++i) {
        double multiple_reference_internal_value;
        auto reference_str = prefix + "multiple_reference_internal[" + std::to_string(i) + "]";
        error_code = alfasim_sdk_api.get_plugin_input_data_quantity(
            ctx,
            &multiple_reference_internal_value,
            get_plugin_id(),
            (reference_str + "->quantity").c_str());
        if (error_code != 0) {
            return error_code;
        }
        multiple_reference_internal_values[i] = multiple_reference_internal_value;
        error_code = alfasim_sdk_api.log_information_message(
            ctx,
            get_plugin_id(),
            std::string(
                "Got quantity (multiple internal reference):"
                + std::to_string(multiple_reference_internal_value))
                .c_str());
        if (error_code != 0) {
            return error_code;
        }
    }

    // Threads Information
    int n_threads = -1;
    error_code = alfasim_sdk_api.get_number_of_threads(ctx, &n_threads);
    if (error_code != 0) {
        return error_code;
    }

    // Setting internal data to each thread
    for (int thread_id = 0; thread_id < n_threads; ++thread_id) {
        auto* template_model = new TemplateModel(
            quantity_value,
            EnumOption(enum_value),
            string_value,
            file_content_value,
            table_value_pressure,
            table_value_temperature,
            reference_tracer_value,
            reference_internal_quantity_value,
            multiple_reference_tracer_values,
            multiple_reference_internal_values);
        error_code = alfasim_sdk_api.set_plugin_data(ctx, plugin_id, template_model, thread_id);
        if (error_code != 0) {
            return error_code;
        }
    }
    return OK;
}

HOOK_FINALIZE(ctx)
{
    // Threads information
    int n_threads = -1;
    int error_code = alfasim_sdk_api.get_number_of_threads(ctx, &n_threads);
    if (error_code != 0) {
        return error_code;
    }
    // Plugin internal data
    for (int thread_id = 0; thread_id < n_threads; ++thread_id) {
        void* template_model = nullptr;
        error_code =
            alfasim_sdk_api.get_plugin_data(ctx, &template_model, get_plugin_id(), thread_id);
        delete template_model;
        if (error_code != 0) {
            return error_code;
        }
    }
    // Unloading ALFAsim-SDK API
    alfasim_sdk_close(&alfasim_sdk_api);
    return OK;
}

HOOK_UPDATE_PLUGINS_SECONDARY_VARIABLES(ctx)
{
    int error_code = -1;
    int size_U = -1;
    int size_E = -1;
    int oil_id = -1;
    error_code = alfasim_sdk_api.get_field_id(ctx, &oil_id, "oil");
    if (error_code != 0) {
        return error_code;
    }
    double* vel;
    VariableScope Fields_OnFaces = { GridScope::FACE,
                                     MultiFieldDescriptionScope::FIELD,
                                     TimestepScope::CURRENT };
    error_code = alfasim_sdk_api.get_simulation_array(
        ctx, &vel, (char*) "U", Fields_OnFaces, oil_id, &size_U);
    if (error_code != 0) {
        return error_code;
    }
    double* kinetic_energy;
    char* name = "kinetic_energy_of_oil";
    int global_idx = 0;
    error_code = alfasim_sdk_api.get_plugin_variable(
        ctx, (void**) (&kinetic_energy), name, global_idx, TimestepScope::CURRENT, &size_E);
    if (error_code != 0) {
        return error_code;
    }
    if (size_U != size_E) {
        return OUT_OF_BOUNDS;
    }
    for (int i = 0; i < size_U; ++i) {
        kinetic_energy[i] = vel[i] * vel[i] / 2.;  // Using [J/kg] to simplify the example
    }
    return OK;
}

HOOK_CALCULATE_MASS_SOURCE_TERM(ctx, mass_source, n_fields, n_control_volumes)
{
    int error_code = -1;
    int extra_field_id = -1;
    error_code = alfasim_sdk_api.get_field_id(ctx, &extra_field_id, "extra");
    if (error_code != 0) {
        return error_code;
    }
    // Convertion from void* to double* and getting the
    // array range related to extra field
    double* extra_mass_source = (double*) mass_source + n_control_volumes * extra_field_id;
    // Make some calculations and add it to extra_mass_source.
    // In this example, we add a mass source of 3.1415 kg/s to all control volumes.
    for (int i = 0; i < n_control_volumes; ++i) {
        extra_mass_source[i] = 3.1415;  // [kg/s]
    }
    return OK;
}
