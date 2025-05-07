#include "traubtx8h_definition.h"

namespace NCParser {

void TraubTX8H_definition::updateData(std::shared_ptr<std::map<int,GCode>> gCodes,
                                 std::shared_ptr<std::map<int,MCode>> mCodes,
                                 std::shared_ptr<std::map<int, FixedCycleDefinition> > fixedCycleDefinition)
{

}

MachineParameters TraubTX8H_definition::getParameters()
{
    MachineParameters param;
    updateParameters(param);
    return param;
}

};
