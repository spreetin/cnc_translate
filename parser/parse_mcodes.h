#ifndef PARSE_MCODES_H
#define PARSE_MCODES_H

#include "parse_node.h"

namespace NCParser {

class parser;

class parse_mcodes
{
public:
    parse_mcodes(parser *parent);

    parse_node *parse(int code);

protected:
    parser *parent;
};

};

#endif // PARSE_MCODES_H
