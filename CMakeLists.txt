cmake_minimum_required(VERSION 3.16)

project(cnc_translate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cnc_translate main.cpp)

target_link_libraries(cnc_translate cnc_parse)

set(
    common_src
    tokens/tokens.h
    lexer/lexer.h lexer/lexer.cpp
    parser/parser.h parser/parser.cpp
    brands/definition_base.h brands/definition_base.cpp
    brands/brands/traub/traubtx8h_definition.h brands/brands/traub/traubtx8h_definition.cpp
    tokens/iso6983.cpp tokens/iso6983.h
    parser/parse_node.h parser/parse_node.cpp
    brands/brands.h
    brands/machine_definitions.h
    brands/brands/traub/traub_definition.h brands/brands/traub/traub_definition.cpp
    brands/parameters.h
    brands/auto_geometry_function.h brands/auto_geometry_function.cpp
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
)

add_library (
    cnc_parse
    ${common_src}
)
add_library (
    cnc_parse_test
    ${common_src}
)
set_target_properties(cnc_parse_test PROPERTIES COMPILE_FLAGS "-DTESTING")

add_subdirectory(tests)

include(GNUInstallDirs)
install(TARGETS cnc_translate
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
