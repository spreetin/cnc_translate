#ifndef DEFINITION_BASE_H
#define DEFINITION_BASE_H

#include "parameters.h"

namespace NCParser {

class definition_base
{
public:
    definition_base() {}

    virtual MachineParameters getParameters(){
        return MachineParameters();
    };

protected:
    virtual void updateParameters(MachineParameters * param) {}
};

};

#endif // DEFINITION_BASE_H
