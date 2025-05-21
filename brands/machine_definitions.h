#ifndef MACHINE_DEFINITIONS_H
#define MACHINE_DEFINITIONS_H

#include <array>
#include "../brands/definition_base.h"
#include "../brands/brands/okuma/okumaosp_p300l.h"
#include "../brands/brands/traub/traubtx8h_definition.h"
#include "../brands/brands.h"

namespace NCParser {

struct machine_definition{
    std::string_view full_name;
    Manufacturers manufacturer;
    std::string_view name;
};



class MachineDefinitions {
public:

    inline static definition_base * getDefinition(Manufacturers manufacturer, std::string_view machine){
        for (int i=0;i<machines.size();i++){
            if (machines.at(i).manufacturer == manufacturer && machines.at(i).name == machine){
                return getDefinition(i);
            }
        }
        return new definition_base();
    };
    inline static definition_base * getDefinition(int machine_id){
        switch (machine_id){
        case 0:
            return new TraubTX8H_definition();
        case 1:
            return new OkumaOSP_P300L();
        default: // ISO 6983-1:2009
            return new definition_base();
        }
    }

private:
    constexpr static std::array<machine_definition, 2> machines {
        machine_definition{"Traub TX8H", Traub, "TX8H"},                // 0
        machine_definition{"Okuma OSP-P300L", Okuma, "OSP-P300L"}       // 1
    };

public:
    inline static constexpr std::array<std::string_view, machines.size()> getMachines(){
        std::array<std::string_view, machines.size()> m;
        for (int i=0;i<machines.size();i++){
            m.at(i) = machines.at(i).full_name;
        }
        return m;
    }

    inline static constexpr Manufacturers getManufacturer(int machine_id){
        if (machine_id >= machines.size()){
            return UnknownBrand;
        }
        return machines.at(machine_id).manufacturer;
    }

    inline static constexpr std::string_view getMachine(int machine_id){
        if (machine_id >= machines.size()){
            return "Unkown";
        }
        return machines.at(machine_id).name;
    }
};

};

#endif // MACHINE_DEFINITIONS_H
