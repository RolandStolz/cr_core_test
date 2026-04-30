
set(lib_files
    ${PROJECT_SOURCE_DIR}/src/structs.h
    ${PROJECT_SOURCE_DIR}/src/structs.c
)

add_library(${c_lib_name}
    ${lib_files}
)

target_compile_options(${c_lib_name} PRIVATE
    -Wall
    -Werror
)
target_compile_features(${c_lib_name} PRIVATE c_std_11)

# Find libxml2
find_package(LibXml2 REQUIRED)

# Include libxml2 headers and link the library
target_include_directories(${c_lib_name} PRIVATE ${LIBXML2_INCLUDE_DIRS})
target_link_libraries(${c_lib_name} PRIVATE ${LIBXML2_LIBRARIES})
target_compile_definitions(${c_lib_name} PRIVATE ${LIBXML2_DEFINITIONS})

target_include_directories(${c_lib_name}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)