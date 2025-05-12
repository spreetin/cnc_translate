#ifndef MACHINE_DEFINITIONS_H
#define MACHINE_DEFINITIONS_H

#include "../brands/definition_base.h"
#include "../brands/brands/okuma/okumaosp_p300l.h"
#include "../brands/brands/traub/traubtx8h_definition.h"
#include "../brands/brands.h"

namespace NCParser {

class MachineDefinitions {
public:
    inline static definition_base * getDefinition(Manufacturers manufacturer, std::string machine){
        switch (manufacturer){
        case Traub:
            if (machine == "TX8H"){
                return new TraubTX8H_definition();
            }
            break;
        case Index:
        case Citizen:
        case Star:
            break;
        case Okuma:
            if (machine == "OSP-P300L"){
                return new OkumaOSP_P300L();
            }
            break;
        }


        return new definition_base();
    };
};

};

#endif // MACHINE_DEFINITIONS_H
