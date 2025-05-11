#ifndef OKUMAOSP_P3__L_H
#define OKUMAOSP_P3__L_H

#include "../../definition_base.h"

namespace NCParser {

class OkumaOSP_P300L : public definition_base
{
public:
    OkumaOSP_P300L() : definition_base() {}

    MachineParameters getParameters() override;
};

}

#endif // OKUMAOSP_P3__L_H
