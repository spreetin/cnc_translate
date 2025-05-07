#include "parse_mcodes.h"
#include "parser.h"

namespace NCParser {

parse_mcodes::parse_mcodes(parser *parent) : parent(parent) {}

parse_node *parse_mcodes::parse(int code)
{
    return new parse_node(Token::m_word, code);
}

};
