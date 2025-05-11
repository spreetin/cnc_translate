#include "traub_definition.h"

namespace NCParser {

void traub_definition::updateParameters(MachineParameters *param)
{
    param->comments.use_parenthesis = true;
}

};

