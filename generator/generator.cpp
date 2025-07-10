#include <sstream>
#include "generator.h"
#include "../brands/machine_definitions.h"
#include "../brands/definition_base.h"
#include "../generator/parse_node_gen.h"

namespace NCParser {

generator::generator(Manufacturers manufacturer, std::string_view machine) {
    this->manufacturer = manufacturer;
    this->machine = machine;
    auto topLayer = std::unique_ptr<definition_base>(MachineDefinitions::getDefinition(manufacturer, machine));
    if (topLayer){
        param = topLayer->getParameters();
    }
    init(param);
}

generator::generator(MachineParameters params) {
    init(params);
}

std::string generator::generate(parse_node_p root)
{
    std::stringstream ss;
    ss << parse_node_gen(&param, root, {}).generate();
    return ss.str();
}

void generator::init(MachineParameters params){
    param = params;
    allowed_multiletter.clear();
    for (auto &item : param.functions.unary){
        allowed_multiletter.insert(item.first);
    }
    for (auto &item : param.functions.binary){
        allowed_multiletter.insert(item.first);
    }
    for (auto &item : param.functions.block){
        allowed_multiletter.insert(item.first);
    }


}

};
