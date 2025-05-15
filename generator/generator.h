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
    generator(Manufacturers manufacturer, std::string_view machine);

    std::string generate(parse_node_p root);

protected:
    MachineParameters param;
    Manufacturers manufacturer;
    std::string_view machine;
};

}

#endif // GENERATOR_H
