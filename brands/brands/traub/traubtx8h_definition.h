#ifndef TRAUBTX8H_DEFINITION_H
#define TRAUBTX8H_DEFINITION_H

#include "../../definition_base.h"
#include "../../parameters.h"

namespace NCParser {

class TraubTX8H_definition : public definition_base
{
public:
    TraubTX8H_definition() : definition_base() {}

    MachineParameters getParameters() override;
};

};

#endif // TRAUBTX8H_DEFINITION_H
