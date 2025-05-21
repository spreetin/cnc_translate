#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include "../brands/parameters.h"
#include "../brands/brands.h"
#include "../parser/parse_node.h"

namespace NCParser {

class generator
{
public:
    generator(Manufacturers manufacturer, std::string_view machine);

    std::string generate(parse_node_p root);

    MachineParameters & getParam(){
        return param;
    }

    [[nodiscard]] std::set<std::string_view> getMultiletter(){
        return allowed_multiletter;
    }

protected:
    MachineParameters param;
    Manufacturers manufacturer;
    std::string_view machine;
    std::set<std::string_view> allowed_multiletter;
};

}

#endif // GENERATOR_H
