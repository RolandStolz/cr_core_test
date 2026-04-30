cmake_minimum_required(VERSION 3.15)
project(test_cr_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(c_lib_name "cr_lib")

option(ENABLE_ASAN "Enable AddressSanitizer" ON)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

add_subdirectory(src)

# Define the executable
add_executable(test_app main.c)
target_link_libraries(test_app PRIVATE ${c_lib_name})
