#include <iostream>
#include <stdexcept>
#include "cli_options.h"
#include "parser/parser.h"
#include "generator/generator.h"

int main(int argc, char *argv[])
{
    cli_options * ops = nullptr;
    try {
        ops = new cli_options(argc, argv);
    } catch (std::runtime_error e){
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    if (ops->help()){
        //std::cout << "CNC Parser and translator" << std::endl
        //          << "Syntax: cnc_translate [-v] [-h] [-in|--input-manufacturer] text [-im|--input-manufacturer]" << std::endl;
        return 0;
    }
    NCParser::parser p((NCParser::Manufacturers)ops->input_manufacturer(), ops->input_machine());
    NCParser::parse_node_p result;
    if (ops->input_files().size()){
        ;
    } else {
        ;
    }
    NCParser::generator g((NCParser::Manufacturers)ops->output_manufacturer(), ops->output_machine());
    if (ops->output_files().size()){
        ;
    } else {
        std::cout << g.generate(result);
    }
    return 0;
}
