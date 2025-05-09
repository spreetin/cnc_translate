#ifndef TRAUBTX8H_DEFINITION_H
#define TRAUBTX8H_DEFINITION_H

#include "../../definition_base.h"
#include "../../parameters.h"

namespace NCParser {

class TraubTX8H_definition : public definition_base
{
public:
    TraubTX8H_definition() : definition_base() {}

    void updateData(std::shared_ptr<std::map<int,GCode>> gCodes,
                    std::shared_ptr<std::map<int,MCode>> mCodes,
                    std::shared_ptr<std::map<int,FixedCycleDefinition>> fixedCycleDefinition) override;
    MachineParameters getParameters() override;
};

};

#endif // TRAUBTX8H_DEFINITION_H
