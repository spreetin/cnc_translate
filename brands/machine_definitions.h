#ifndef MACHINE_DEFINITIONS_H
#define MACHINE_DEFINITIONS_H

#include "../brands/definition_base.h"
#include "../brands/brands/traub/traubtx8h_definition.h"
#include "../brands/brands.h"

namespace NCParser {

class MachineDefinitions {
public:
    inline static definition_base * getDefinition(Manufacturers manufacturer, std::string machine){
        if (manufacturer == Manufacturers::Traub && (machine == "TX8H")){
            return new TraubTX8H_definition();
        }
        return new definition_base();
    };
};

};

#endif // MACHINE_DEFINITIONS_H
