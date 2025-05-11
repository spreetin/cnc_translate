#include "okumaosp_p300l.h"

namespace NCParser {

MachineParameters OkumaOSP_P300L::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings

    // Axis

    // Define changed default word meanings
    param.parameter_defaults['O'] = param_prg_name;

    // Variables

    // Functions

    // Unset default G-codes that are not valid

    // Set all defined G-codes

    // Define M-codes


    return param;
}

}
