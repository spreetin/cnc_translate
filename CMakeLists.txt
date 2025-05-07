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
    parser/parse_gcodes.h parser/parse_gcodes.cpp
    parser/parse_node.h parser/parse_node.cpp

    parser/parse_mcodes.h parser/parse_mcodes.cpp
    brands/brands.h
    brands/machine_definitions.h
    brands/brands/traub/traub_definition.h brands/brands/traub/traub_definition.cpp
)

include(GNUInstallDirs)
install(TARGETS cnc_translate
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
