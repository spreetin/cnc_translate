#include <iostream>
#include <sstream>
#include "generator.h"
#include "../brands/definition_base.h"
#include "../errors/errors.h"
#include "../generator/parse_node_gen.h"

namespace NCParser {

generator::generator(Manufacturers manufacturer, std::string machine) {
    this->manufacturer = manufacturer;
    this->machine = machine;
    auto topLayer = std::unique_ptr<definition_base>(MachineDefinitions::getDefinition(manufacturer, machine));
    if (topLayer){
        param = topLayer->getParameters();
    }
}

std::string generator::generate(std::vector<parse_node*> root)
{
    std::stringstream ss;
    ss << "%\n";
    // TODO Write subsystem definition and other boilerplate
    int block_no = 1;
    for (auto block : root){
        if (block){
            ss << parse_node_gen::generate(block, &param);
        } else {
            std::cerr << (error{error::Generating, block_no, -1,
                                "Block pointer is null"}).to_string();
        }
        ss << "\n";
        block_no++;
    }
    return ss.str();
}

};
