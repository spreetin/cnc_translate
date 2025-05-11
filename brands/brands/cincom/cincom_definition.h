#ifndef CINCOM_DEFINITION_H
#define CINCOM_DEFINITION_H

#include "../../definition_base.h"

namespace NCParser {

class cincom_definition : public definition_base
{
public:
    cincom_definition() : definition_base() {}

    virtual MachineParameters getParameters() override {
        return MachineParameters();
    }


protected:
    void updateParameters(MachineParameters * param) override;
};

};

#endif // CINCOM_DEFINITION_H
