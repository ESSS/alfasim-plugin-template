import alfasim_sdk
from alfasim_sdk import AddField
from alfasim_sdk import AddPhase
from alfasim_sdk import Boolean
from alfasim_sdk import container_model
from alfasim_sdk import Context
from alfasim_sdk import data_model
from alfasim_sdk import Enum
from alfasim_sdk import ErrorMessage
from alfasim_sdk import FileContent
from alfasim_sdk import GAS_PHASE
from alfasim_sdk import Location
from alfasim_sdk import MultipleReference
from alfasim_sdk import OIL_LAYER
from alfasim_sdk import OIL_PHASE
from alfasim_sdk import Quantity
from alfasim_sdk import Reference
from alfasim_sdk import Scope
from alfasim_sdk import SecondaryVariable
from alfasim_sdk import SOLID_PHASE
from alfasim_sdk import String
from alfasim_sdk import Table
from alfasim_sdk import TableColumn
from alfasim_sdk import TracerType
from alfasim_sdk import UpdateLayer
from alfasim_sdk import Visibility


# Declare a Model/Property that will be used inside the Template Model as Reference
@data_model(icon="", caption="Custom Model")
class CustomModel:
    name = String(value="Template Model Name", caption="Name")
    quantity = Quantity(value=1, unit="m", caption="Property")


# Declaring a container that can be used to hold a Custom Property or Model
# This is useful when we want to create a data structure that will be used inside the TemplateModel
@container_model(icon="", caption="Custom Property", model=CustomModel)
class CustomModelContainer:
    pass


def my_custom_enable_expr_for_file_content(attr, ctx):
    return attr.enum == "VALUE_B"


@data_model(icon="", caption="Template Model")
class TemplateModel:
    # This first string is used to name the Model in the Tree
    name = String(value="Template Model Name", caption="Name")
    quantity = Quantity(value=1, unit="m", caption="Quantity")
    enum = Enum(values=["VALUE_A", "VALUE_B"], caption="ComboBox")
    boolean = Boolean(value=False, caption="Click Me!")
    string = String(
        value="Foo", caption="Write something", enable_expr=lambda self, ctx: self.boolean
    )
    file_content = FileContent(
        caption="File Path Caption", visible_expr=my_custom_enable_expr_for_file_content
    )
    table = Table(
        rows=[
            TableColumn(id="pressure", value=Quantity(value=1, unit="bar", caption="Pressure")),
            TableColumn(id="temperature", value=Quantity(value=2, unit="K", caption="Temperature")),
        ],
        caption="Table",
    )
    reference_tracer = Reference(ref_type=TracerType, caption="Tracer Reference")
    reference_internal = Reference(
        container_type="CustomModelContainer",
        ref_type=CustomModel,
        caption="Custom Model Reference",
        tooltip="This is a internal reference that can be used to select ONE of the Custom Models defined",
    )
    multiple_reference_tracer = MultipleReference(
        ref_type=TracerType, caption="Multiple Tracer Reference"
    )
    multiple_reference_internal = MultipleReference(
        ref_type=CustomModel,
        container_type="CustomModelContainer",
        caption="Multiple Internal Reference",
        tooltip="This is a internal reference that can be used to select one or more of the Custom Models defined",
    )


# Declaring a container that can hold any number of configurations of the TemplateModel.
# This is useful for saving the time of declaring a model and then changing when trying to test
# a different configuration.
@container_model(icon="", caption="Models List", model=TemplateModel)
class ModelSelector:
    selected_model = Reference(
        ref_type=TemplateModel,
        container_type="ModelSelector",
        caption="Selected Model",
    )


@alfasim_sdk.hookimpl
def alfasim_get_data_model_type():
    return [ModelSelector, CustomModelContainer]


@alfasim_sdk.hookimpl
def alfasim_configure_fields(ctx: Context) -> list[AddField]:
    return [AddField(name="extra")]


@alfasim_sdk.hookimpl
def alfasim_configure_layers(ctx: Context) -> list[UpdateLayer]:
    return [UpdateLayer(name=OIL_LAYER, additional_fields=["extra"])]


@alfasim_sdk.hookimpl
def alfasim_configure_phases(ctx: Context) -> list[AddPhase]:
    return [AddPhase(name="extra", fields=["extra"], primary_field="extra")]


@alfasim_sdk.hookimpl
def alfasim_get_user_defined_tracers_from_plugin():
    return ["my_tracer"]


@alfasim_sdk.hookimpl
def alfasim_get_phase_properties_calculated_from_plugin():
    return [SOLID_PHASE]


@alfasim_sdk.hookimpl
def alfasim_get_phase_interaction_properties_calculated_from_plugin():
    return [(SOLID_PHASE, GAS_PHASE), (SOLID_PHASE, OIL_PHASE)]


@alfasim_sdk.hookimpl
def alfasim_get_additional_variables():
    return [
        SecondaryVariable(
            name="kinetic_energy_of_oil",
            caption="Kinetic Energy of Oil",
            unit="J/kg",
            visibility=Visibility.Output,
            location=Location.Face,
            multifield_scope=Scope.Global,
            checked_on_gui_default=True,
        )
    ]


@alfasim_sdk.hookimpl
def alfasim_get_status(ctx):
    from alfasim_sdk import WarningMessage

    result = []

    model_selector_container = ctx.GetModel("ModelSelector")
    if not model_selector_container.selected_model:
        result.append(
            alfasim_sdk.ErrorMessage(model_name="ModelSelector", message="Template Model not set.")
        )

    model = ctx.GetModel("TemplateModel")
    if model.boolean:
        result.append(
            WarningMessage(
                model_name=model.name,
                message="Warning Message example: triggered when boolean is True",
            )
        )

    if model.quantity.GetValue("m") < 0:
        result.append(
            ErrorMessage(
                model_name=model.name,
                message="Error Message example: triggered when quantity is below zero",
            )
        )

    return result
