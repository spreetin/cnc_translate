#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include "../brands/machine_definitions.h"
#include "../brands/parameters.h"
#include "../brands/brands.h"
#include "../parser/parse_node.h"

namespace NCParser {

class generator
{
public:
    generator(Manufacturers manufacturer, std::string machine);

    std::string generate(std::vector<parse_node*> root);

protected:
    std::string expr(parse_node *node);

    MachineParameters param;
    Manufacturers manufacturer;
    std::string machine;
};

}

#endif // GENERATOR_H
