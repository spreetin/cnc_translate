#include "libcnc_parse.h"
#include "generator/generator.h"
#include "parser/parser.h"

namespace NCParser {

parse_node_p parse(Manufacturers man, std::string_view machine, std::string text){
    parser * p = new parser(man, machine);
    p->parse(text);
    auto r = p->result();
    return r;
}

std::string generate(Manufacturers man, std::string_view machine, parse_node_p nodes){
    generator * g = new generator(man, machine);
    auto r = g->generate(nodes);
    return r;
}

}
