#include "traub_definition.h"

namespace NCParser {

void traub_definition::updateData(std::shared_ptr<std::map<int, GCode> > gCodes, std::shared_ptr<std::map<int, MCode> > mCodes, std::shared_ptr<std::map<int, FixedCycleDefinition> > fixedCycleDefinition)
{

}

void traub_definition::updateParameters(MachineParameters *param)
{
    param->comments.use_parenthesis = true;
}

};

