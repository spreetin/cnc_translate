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
    param.parameter.subtypes['X'] = {1, 2, 3};
    param.parameter.subtypes['Y'] = {1, 2, 3};
    param.parameter.subtypes['Z'] = {1, 2, 3};
    param.parameter.subtypes['U'] = {1, 2, 3};
    param.parameter.subtypes['W'] = {1, 2, 3};
    param.parameter.subtypes['V'] = {1, 2, 3};
    param.parameter.subtypes['A'] = {3, 4, 7};
    param.parameter.subtypes['B'] = {1};
    param.parameter.subtypes['C'] = {1, 2};
    param.parameter.subtypes['H'] = {1, 2};

    // Spindles
    param.spindles[1] = MainSpindle;
    param.spindles[2] = BackSpindle;
    param.spindles[3] = GangToolSpindle;
    param.spindles[4] = TurretToolSpindle;
    param.spindles[5] = BackRotaryToolSpindle;
    param.spindles[9] = GuideBushingDrive;

    // Define changed default word meanings
    param.parameter.defaults['O'] = param_prg_name;
    param.parameter.defaults['L'] = param_queueing_numbered;

    // Variables

    // Functions

    // Unset default G-codes that are not valid

    // Set all defined G-codes
    param.g[g_rapid_positioning] = {0, gmode_motion};
    param.g[g_linear_interpolation] = {1, gmode_motion};
    param.g[g_circular_interpolation_cw] = {2, gmode_motion};
    param.g[g_circular_interpolation_ccw] = {3, gmode_motion};
    param.g[g_dwell] = {4, gmode_nonmodal};
    param.g[g_exact_stop] = {9, gmode_nonmodal};
    param.g[g_milling_interpolation_on] = {"12.1", gmode_milling_interpolation};
    param.g[g_milling_interpolation_off] = {"13.1", gmode_milling_interpolation};
    param.g[g_yz_cylindrical_plane_selection] = {16, gmode_plane_selection};
    param.g[g_xy_plane_selection] = {17, gmode_plane_selection};
    param.g[g_zx_plane_selection] = {18, gmode_plane_selection};
    param.g[g_yz_plane_selection] = {19, gmode_plane_selection};
    param.g[g_thread_cutting_constant_lead] = {32, gmode_motion};
    param.g[g_thread_cutting_variable_lead] = {34, gmode_motion};
    param.g[g_thread_cutting_circular_cw] = {35, gmode_motion};
    param.g[g_thread_cutting_circular_ccw] = {36, gmode_motion};

    // Define M-codes

    return param;
}

}
