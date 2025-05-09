#ifndef DEFINITION_BASE_H
#define DEFINITION_BASE_H

#include <map>
#include <memory>
#include "../tokens/tokens.h"
#include "parameters.h"

namespace NCParser {

class definition_base
{
public:
    definition_base() {}

    virtual void updateData(std::shared_ptr<std::map<int,GCode>> gCodes,
                            std::shared_ptr<std::map<int,MCode>> mCodes,
                            std::shared_ptr<std::map<int,FixedCycleDefinition>> fixedCycleDefinition){};
    virtual MachineParameters getParameters(){
        return MachineParameters();
    };

protected:
    virtual void updateParameters(MachineParameters * param) {}
};

};

#endif // DEFINITION_BASE_H
