find_program(VENV_PYTHON
    NAMES python python3
    HINTS "${CMAKE_SOURCE_DIR}/.venv/bin"
    NO_DEFAULT_PATH
    REQUIRED
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_cr_built.stamp
    COMMAND ${VENV_PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/_build.py ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/_cr_built.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/_build.py
    COMMENT "Building cffi extension"
)

add_custom_target(cr_cffi ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/_cr_built.stamp
)

install(CODE "
    file(GLOB cffi_so \"${CMAKE_CURRENT_BINARY_DIR}/_c_python_module*.so\")
    file(INSTALL \${cffi_so} DESTINATION ${CMAKE_INSTALL_PREFIX})
")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/c_python_module.py DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/bindings/python/c_api/c_python_module.pyi DESTINATION .)