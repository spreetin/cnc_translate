#ifndef CINCOMM32_4M8_DEFINITION_H
#define CINCOMM32_4M8_DEFINITION_H

#include "../../definition_base.h"

namespace NCParser {

class CincomM32_4M8_definition : public definition_base
{
public:
    CincomM32_4M8_definition() : definition_base() {}

    MachineParameters getParameters() override;
};

};

#endif // CINCOMM32_4M8_DEFINITION_H
