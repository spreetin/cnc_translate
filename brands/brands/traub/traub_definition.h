#ifndef TRAUB_DEFINITION_H
#define TRAUB_DEFINITION_H

#include "../../definition_base.h"

namespace NCParser {

class traub_definition : public definition_base
{
public:
    traub_definition() : definition_base() {}

    virtual void updateData(std::shared_ptr<std::map<int,GCode>> gCodes,
                    std::shared_ptr<std::map<int,MCode>> mCodes,
                    std::shared_ptr<std::map<int,FixedCycleDefinition>> fixedCycleDefinition) override;
    virtual MachineParameters getParameters() override;

protected:
    MachineParameters updateParameters(MachineParameters param) override;
};

};

#endif // TRAUB_DEFINITION_H
