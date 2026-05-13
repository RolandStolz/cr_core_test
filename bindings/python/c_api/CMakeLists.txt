find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
message(STATUS "Python_EXECUTABLE: ${Python_EXECUTABLE}")

Python_add_library(c_python_module MODULE 
    ${PROJECT_SOURCE_DIR}/bindings/python/c_api/cr_module.c
)

target_link_libraries(c_python_module PRIVATE ${c_lib_name} Python::Module)

install(TARGETS c_python_module DESTINATION .)

# install stubs
install(FILES ${PROJECT_SOURCE_DIR}/bindings/python/c_api/c_python_module.pyi DESTINATION .)