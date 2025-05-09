cmake_minimum_required(VERSION 3.16)

project(cnc_translate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cnc_translate main.cpp
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
)

include(GNUInstallDirs)
install(TARGETS cnc_translate
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
