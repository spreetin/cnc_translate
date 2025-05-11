#include "cincomm32_4m8_definition.h"

namespace  NCParser {

MachineParameters CincomM32_4M8_definition::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings
    param.general.has_nose_radius_compenstion = true;
    param.queueing.queueing_single_char = '!';
    param.queueing.queueing_numbered_single_char = 'L';

    // Axis
    param.parameter_subtypes['X'] = {1, 2, 3};
    param.parameter_subtypes['Y'] = {1, 2, 3};
    param.parameter_subtypes['Z'] = {1, 2, 3};
    param.parameter_subtypes['U'] = {1, 2, 3};
    param.parameter_subtypes['W'] = {1, 2, 3};
    param.parameter_subtypes['V'] = {1, 2, 3};
    param.parameter_subtypes['A'] = {3, 4, 7};
    param.parameter_subtypes['B'] = {1};
    param.parameter_subtypes['C'] = {1, 2};
    param.parameter_subtypes['H'] = {1, 2};

    // Spindles
    param.spindles[1] = MainSpindle;
    param.spindles[2] = BackSpindle;
    param.spindles[3] = GangToolSpindle;
    param.spindles[4] = TurretToolSpindle;
    param.spindles[5] = BackRotaryToolSpindle;
    param.spindles[9] = GuideBushingDrive;

    // Define changed default word meanings
    param.parameter_defaults['O'] = param_prg_name;
    param.parameter_defaults['L'] = param_queueing_numbered;

    // Variables

    // Functions

    // Unset default G-codes that are not valid

    // Set all defined G-codes
    param.g[rapid_positioning] = {0, gmode_motion};
    param.g[linear_interpolation] = {1, gmode_motion};
    param.g[circular_interpolation_cw] = {2, gmode_motion};
    param.g[circular_interpolation_ccw] = {3, gmode_motion};
    param.g[dwell] = {4, gmode_nonmodal};
    param.g[exact_stop] = {9, gmode_nonmodal};
    param.g[milling_interpolation_on] = {"12.1", gmode_milling_interpolation};
    param.g[milling_interpolation_off] = {"13.1", gmode_milling_interpolation};
    param.g[yz_cylindrical_plane_selection] = {16, gmode_plane_selection};
    param.g[xy_plane_selection] = {17, gmode_plane_selection};
    param.g[zx_plane_selection] = {18, gmode_plane_selection};
    param.g[yz_plane_selection] = {19, gmode_plane_selection};
    param.g[thread_cutting_constant_lead] = {32, gmode_motion};
    param.g[thread_cutting_variable_lead] = {34, gmode_motion};
    param.g[thread_cutting_circular_cw] = {35, gmode_motion};
    param.g[thread_cutting_circular_ccw] = {36, gmode_motion};

    // Define M-codes

    return param;
}

}
