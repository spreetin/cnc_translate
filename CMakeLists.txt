cmake_minimum_required(VERSION 3.16)

project(cnc_translate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(
    cnc_translate
    main.cpp
    cli_options.h cli_options.cpp
)

target_link_libraries(
    cnc_translate
    cnc_parse_static
)

set(
    common_src
    tokens/tokens.h
    lexer/lexer.h lexer/lexer.cpp
    parser/parser.h parser/parser.cpp
    brands/definition_base.h brands/definition_base.cpp
    brands/brands/traub/traubtx8h_definition.h brands/brands/traub/traubtx8h_definition.cpp
    parser/parse_node.h parser/parse_node.cpp
    brands/brands.h
    brands/machine_definitions.h
    brands/brands/traub/traub_definition.h brands/brands/traub/traub_definition.cpp
    brands/parameters.h

    brands/brands/traub/traub_auto_geometry_functions.h brands/brands/traub/traub_auto_geometry_functions.cpp
    brands/parameter_definitions.h
    tokens/g_codes.h
    tokens/m_codes.h
    errors/errors.h
    brands/brands/cincom/cincomm32_4m8_definition.h brands/brands/cincom/cincomm32_4m8_definition.cpp
    brands/brands/cincom/cincom_definition.h brands/brands/cincom/cincom_definition.cpp
    brands/brands/okuma/okumaosp_p300l.h brands/brands/okuma/okumaosp_p300l.cpp
    generator/generator.h generator/generator.cpp
    generator/parse_node_gen.h generator/parse_node_gen.cpp
    tokens/g_code_description.h
    tokens/m_code_descriptions.h
    regex/regex_wrapper.h regex/regex_wrapper.cpp
    generator/generate_g_milling.cpp
    generator/generate_g_threads.cpp
    generator/generate_g_tools.cpp
    generator/generate_g_settings.cpp
    generator/generate_g_cutting.cpp
    tokens/parameter_descriptions.h
    macros/contracts.h

    # Compile time regular expressions library
    ctre/ctre.hpp ctre/ctre-unicode.hpp ctre/unicode-db.hpp
)

add_library (
    cnc_parse_internal
    OBJECT
    ${common_src}
)

set_property(
    TARGET cnc_parse_internal
    PROPERTY POSITION_INDEPENDENT_CODE 1
)

add_library (
    cnc_parse_shared
    SHARED
    $<TARGET_OBJECTS:cnc_parse_internal>
)
add_library (
    cnc_parse_static
    STATIC
    $<TARGET_OBJECTS:cnc_parse_internal>
)

add_library (
    cnc_parse_test
    ${common_src}
)

add_subdirectory(tests)
add_subdirectory(cnc_gui)

set_target_properties(cnc_parse_test PROPERTIES COMPILE_FLAGS "-DTESTING")

include(GNUInstallDirs)
install(TARGETS cnc_translate
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
