#include "traub_definition.h"

namespace NCParser {

void traub_definition::updateData(std::shared_ptr<std::map<int, GCode> > gCodes, std::shared_ptr<std::map<int, MCode> > mCodes, std::shared_ptr<std::map<int, FixedCycleDefinition> > fixedCycleDefinition)
{

}

MachineParameters traub_definition::getParameters()
{
    return updateParameters(MachineParameters());
}

MachineParameters traub_definition::updateParameters(MachineParameters param)
{
    param.comments.use_parenthesis = true;
    param.L.is_variable = true;
    param.O.is_prg_name = true;
    return param;
}

};

