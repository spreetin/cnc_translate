#ifndef TRAUB_DEFINITION_H
#define TRAUB_DEFINITION_H

#include "../../definition_base.h"

namespace NCParser {

class traub_definition : public definition_base
{
public:
    traub_definition() : definition_base() {}

    virtual MachineParameters getParameters() override {
        return MachineParameters();
    };

protected:
    void updateParameters(MachineParameters * param) override;
};

};

#endif // TRAUB_DEFINITION_H
