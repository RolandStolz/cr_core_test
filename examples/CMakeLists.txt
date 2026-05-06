# Define the executable
add_executable(test_app main.c)
target_link_libraries(test_app PRIVATE ${c_lib_name})