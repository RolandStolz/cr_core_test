cmake_minimum_required(VERSION 3.15)
project(test_cr_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(c_lib_name "cr_lib")


option(python_bind_with_c_api "Build Python bindings using the C API" OFF)


option(ENABLE_ASAN "Enable AddressSanitizer" ON)
if(ENABLE_ASAN AND NOT SKBUILD)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

add_subdirectory(src)
add_subdirectory(examples)

if (python_bind_with_c_api)
    add_subdirectory(bindings/python/c_api)
else ()
    add_subdirectory(bindings/python/cffi)
endif ()
