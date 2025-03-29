add_library(template SHARED template.cpp hook_specs.h TemplateModel.cpp)
target_include_directories(template PUBLIC ${SDK_INCLUDE_DIR})
install(TARGETS template EXPORT ${PROJECT_NAME}_export DESTINATION ${ARTIFACTS_DIR})
