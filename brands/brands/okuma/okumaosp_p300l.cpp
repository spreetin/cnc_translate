#include "okumaosp_p300l.h"

namespace NCParser {

MachineParameters OkumaOSP_P300L::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings
    param.general.has_nose_radius_compenstion = true;
    param.maths.use_block_parenthesis = true;
    param.maths.hexadecimal = '$';
    param.queueing.queueing_numbered_single_char = 'P';
    param.queueing.queueing_string = "M10";
    param.subsystem.symbol = 'G';
    param.subsystem.number = 2;

    // Axis
    param.parameter.subtypes['X'] = {1};
    param.parameter.subtypes['Y'] = {1};
    param.parameter.subtypes['Z'] = {1};
    param.parameter.subtypes['C'] = {1};
    param.parameter.is_diameter.insert('X');

    // Define changed default word meanings
    param.parameter.defaults['A'] = param_undefined;
    param.parameter.defaults['B'] = param_undefined;
    param.parameter.defaults['C'] = param_c_axis;
    param.parameter.defaults['D'] = param_undefined;
    param.parameter.defaults['E'] = param_undefined;
    param.parameter.defaults['H'] = param_undefined;
    param.parameter.defaults['L'] = param_circle_radius;
    param.parameter.defaults['P'] = param_queueing_numbered;
    param.parameter.defaults['S'] = param_undefined;
    param.parameter.defaults['O'] = param_prg_name;
    param.parameter.defaults['U'] = param_undefined;
    param.parameter.defaults['W'] = param_undefined;

    param.parameter.defaults_multiletter["QA"] = param_c_axis_rotation;
    param.parameter.defaults_multiletter["SA"] = param_c_axis_rpm;
    param.parameter.defaults_multiletter["SB"] = param_tool_rpm;
    param.parameter.defaults_multiletter["TL"] = param_tool_preparation;
    param.parameter.defaults_multiletter["TD"] = param_tool_preparation;

    param.parameter.always_useable = {'S', 'T'}; // SB, TL, TD, TS

    // Variables
    param.subprograms.available_variables = {Token_OSPP300L_1};
    param.variables.available_variables = {Token_OSPP300L_1, Token_OSPP300L_2};

    // Functions
    param.functions.unary = {{"NOT", f_not_b}, {"SIN", f_sine}, {"COS", f_cosine}, {"TAN", f_tangent},
                             {"ATAN", f_arctangent}, {"SQRT", f_square_root}, {"ABS", f_abs},
                             {"BIN", f_dec_to_bin}, {"BCD", f_bin_to_dec}, {"ROUND", f_round},
                             {"FIX", f_floor}, {"FUP", f_ceil}, {"DROUND", f_round_decimals},
                             {"DFIX", f_floor_decimals}, {"DFUP", f_ceil_decimals}, {"GOTO", f_jump},
                             {"IF", f_if}};
    param.functions.binary = {{"EOR", f_xor_b}, {"OR", f_or_b}, {"AND", f_and_b}};
    param.functions.coordinate = {{"ATAN2", f_arctangent2}, {"MOD", f_mod}};

    // Tool data
    param.tool_selection.parts = {
        {"T",
            {{param_tool_selection_nrc, 2},
            {param_tool_selection_tool, 2},
             {param_tool_selection_offset, 2}}},
        {"TL",
            {{param_tool_selection_nrc, 3},
            {param_tool_selection_tool, 3}}},
        {"TD",
            {{parma_tool_selection_pos, 2},
            {param_tool_selection_tool, 4}}},
    };

    // Unset default G-codes that are not valid
    param.g[g_dimension_input_inch] = {};
    param.g[g_dimension_input_metric] = {};

    // Set all defined G-codes
    param.g[g_rapid_positioning] = {0, gmode_motion,
            {'X', 'Z', 'C'}};
    param.g[g_linear_interpolation] = {1, gmode_motion,
            {'X', 'Z', 'C', 'F', 'A'},
            {{'A', param_angle}}};
    param.g[g_circular_interpolation_cw] = {2, gmode_motion,
            {'X', 'Z', 'I', 'K', 'Y', 'L', 'F'},
            {{'I', param_circle_center_x},
             {'K', param_circle_center_z}}};
    param.g[g_circular_interpolation_ccw] = {3, gmode_motion,
            {'X', 'Z', 'I', 'K', 'Y', 'L', 'F'},
            {{'I', param_circle_center_x},
             {'K', param_circle_center_z}}};
    param.g[g_use_subsystem_a] = {13};
    param.g[g_use_subsystem_b] = {14};
    param.g[g_xy_plane_selection] = {17, gmode_plane_selection};
    param.g[g_zx_plane_selection] = {18, gmode_plane_selection};
    param.g[g_chuck_transfer_under_torque] = {22, -1,
            {'Z', 'D', 'L', 'F'}}; // Parameter PZ
    param.g[g_torque_limit_chuck_transfer_off] = {28};
    param.g[g_torque_limit_chuck_transfer] = {29, -1,
            {'P', 'Z', 'W'}}; // Needs more work
    // 31 - as 33 but not lengthwise?
    param.g[g_thread_cutting_end_face] = {32, gmode_motion,
            {'X', 'Z', 'F', 'I', 'A', 'E', 'K', 'L', 'J', 'C'},
            {{'F', param_thread_lead},
             {'I', param_conical_amount},
             {'A', param_conical_angle},
             {'E', param_thread_lead_change},
             {'K', param_thread_start_offset},
             {'L', param_thread_champfer_end_length},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    param.g[g_thread_cutting_variable_lead] = {33, gmode_motion,
            {'X', 'Z', 'F', 'I', 'A', 'E', 'K', 'L', 'J', 'C'},
            {{'F', param_thread_lead},
             {'I', param_conical_amount},
             {'A', param_conical_angle},
             {'E', param_thread_lead_change},
             {'K', param_thread_start_offset},
             {'L', param_thread_champfer_end_length},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    param.g[g_thread_cutting_multistep] = {33, gmode_motion,
            {'X', 'Z', 'F', 'E', 'J', 'C'},
            {{'F', param_thread_lead},
             {'E', param_thread_lead_change},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    param.g[g_cutter_compensation_cancel] = {40};
    param.g[g_cutter_compensation_left] = {41};
    param.g[g_cutter_compensation_right] = {42};
    param.g[g_zero_set] = {50, -1,
            {'X', 'Z', 'C'},
            {{'X', param_set_x_value_on_current},
             {'Z', param_set_z_value_on_current},
             {'C', param_set_c_value_on_current}}};
    param.g[g_rpm_limit] = {50, -1,
            {'S'},
            {{'S', param_rpm_max}}};
    param.g[g_servo_error_compensation_off] = {64};
    param.g[g_servo_error_compensation_on] = {65};
    param.g[g_thread_cutting_cycle] = {71, gmode_motion,
            {'X', 'Z', 'A', 'I', 'B', 'D', 'U', 'H', 'L', 'E', 'F', 'J', 'M', 'Q'},
            {{'A', param_conical_angle},
             {'I', param_conical_amount},
             {'B', param_angle},
             {'D', param_thread_depth_first_cut},
             {'U', param_margin},
             {'H', param_thread_depth},
             {'L', param_thread_champfer_end_length},
             {'E', param_thread_lead_change},
             {'F', param_thread_lead},
             //{'M', param_thread_champfer_end_length}, Special
             //{'Q', param_thread_champfer_end_length}, Special
             {'J', param_thread_lead_count},
             {'C', param_thread_phase_difference}}};
    param.g[g_thread_cutting_cycle_end_face] = {72, gmode_motion,
            {'X', 'Z', 'A', 'I', 'B', 'D', 'U', 'H', 'L', 'E', 'F', 'J', 'M', 'Q'},
            {{'A', param_conical_angle},
             {'I', param_conical_amount},
             {'B', param_angle},
             {'D', param_thread_depth_first_cut},
             {'U', param_margin},
             {'H', param_thread_depth},
             {'L', param_thread_champfer_end_length},
             {'E', param_thread_lead_change},
             {'F', param_thread_lead},
             //{'M', param_thread_champfer_end_length}, Special
             //{'Q', param_thread_champfer_end_length}, Special
             {'J', param_thread_lead_count},
             {'C', param_thread_phase_difference}}};
    param.g[g_wedge_cut_cycle_lengthwise] = {73, gmode_motion,
            {'X', 'Z', 'I', 'K', 'D', 'L', 'E', 'T'},
            {{'D', param_milling_depth},
             // I - kör i x-axelns riktning
             // K - kör i z-axelns riktning
             {'L', param_milling_depth_tool_removal},
             {'E', param_dwell_time},
             {'T', param_tool_selection_offset}}};
    param.g[g_drill_cycle_z_axis] = {74, gmode_motion,
            {'X', 'Z', 'I', 'K', 'D', 'L', 'E', 'T'},
            {{'D', param_milling_depth},
             // I - kör i x-axelns riktning
             // K - kör i z-axelns riktning
             {'L', param_milling_depth_tool_removal},
             {'E', param_dwell_time},
             {'T', param_tool_selection_offset}}};
    param.g[g_transfer_chamfer] = {75, -1,
            {'L'},
            {{'L', param_transfer_chamfer}}, {}, true};
    param.g[g_transfer_radius] = {76, -1,
            {'L'},
            {{'L', param_transfer_chamfer}}, {}, true};
    param.g[g_thread_cutting_circular_cw] = {77, gmode_motion,
            {'X', 'Z', 'K', 'F'},
            {{'X', param_x_start},
             {'K', param_rapid_feed},
             {'F', param_feed}}};
    param.g[g_thread_cutting_circular_ccw] = {78, gmode_motion,
            {'X', 'Z', 'K', 'F'},
            {{'X', param_x_start},
             {'K', param_rapid_feed},
             {'F', param_feed}}};
    // G80, G81, G82, G83, G84, G85, G86, G87, G88 - automatiska skrubbcykler
    param.g[g_feed_per_minute] = {94, gmode_feed_rate,
            {'F'},
            {{'F', param_feed}}};
    param.g[g_feed_per_revolution] = {95, gmode_feed_rate,
            {'F'},
            {{'F', param_feed}}};
    param.g[g_constant_surface_speed] = {96, gmode_spindle_speed,
            {'S'},
            {{'S', param_feed}}};
    param.g[g_revolutions_per_minute] = {97, gmode_spindle_speed,
            {'S'},
            {{'S', param_rpm}}};
    param.g[g_milling_line_end_face] = {101, gmode_motion,
            {'X', 'Z', 'C', 'F'}};
    param.g[g_milling_curve_cw_end_face] = {102, gmode_motion,
            {'X', 'Z', 'C', 'L', 'F'}};
    param.g[g_milling_curve_ccw_end_face] = {103, gmode_motion,
            {'X', 'Z', 'C', 'L', 'F'}};
    param.g[g_xz_cylindrical_plane_selection] = {119, gmode_plane_selection};
    param.g[g_circular_interpolation_cw_milling_on_turning] = {132, gmode_motion,
            {'Z', 'C', 'L', 'F'}};
    param.g[g_circular_interpolation_ccw_milling_on_turning] = {133, gmode_motion,
            {'Z', 'C', 'L', 'F'}};
    param.g[g_coordinate_cartesian_to_polar_off] = {136};
    param.g[g_coordinate_cartesian_to_polar] = {137, -1,
            {'C'}};
    param.g[g_fixed_cycle_cancel] = {180};

    param.g[g_auto_geometry_function] = {std::set<int>{
            178, 179, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190}};

    // Define M-codes
    param.m[m_program_stop] = {0};
    param.m[m_optional_stop] = {1};
    param.m[m_end_of_program] = {2};
    param.m[m_spindle_cw] = {3};
    param.m[m_spindle_ccw] = {4};
    param.m[m_spindle_off] = {5};
    param.m[m_queueing] = {10};
    // M13
    param.m[m_c_axis_rotation_right] = {15};
    param.m[m_c_axis_rotation_left] = {16};
    // M19 spindelorientering
    param.m[m_double_dock_barrier_off] = {20};
    param.m[m_double_dock_barrier_on] = {21};
    param.m[m_thread_chamfer_off] = {22};
    param.m[m_thread_chamfer_on] = {23};
    param.m[m_chuck_barrier_off] = {24};
    param.m[m_chuck_barrier_on] = {25};
    param.m[m_thread_pitch_axis_z] = {26},
    param.m[m_thread_pitch_axis_x] = {27},
    param.m[m_end_of_data] = {30};
    param.m[m_thread_cut_straight_left] = {32};
    param.m[m_thread_cut_zigzag] = {33};
    param.m[m_thread_cut_straight_right] = {34};
    param.m[m_spindle_gear_neutral] = {40};
    param.m[m_spindle_gear_1] = {41};
    param.m[m_spindle_gear_2] = {42};
    param.m[m_spindle_gear_3] = {43};
    param.m[m_spindle_gear_4] = {44};
    param.m[m_bypass_override_off] = {48};
    param.m[m_bypass_override] = {49};
    param.m[m_nubdock_retract] = {55};
    param.m[m_nubdock] = {56};
    param.m[m_feed_rate_wait] = {60};
    param.m[m_feed_rate_nowait] = {61};
    param.m[m_spindle_rotation_nowait] = {63};
    param.m[m_thread_cycle_schema_1] = {73}; // Need special code
    param.m[m_thread_cycle_schema_2] = {74}; // Need special code
    param.m[m_thread_cycle_schema_3] = {75}; // Need special code
    param.m[m_clamp_workpiece] = {83};
    param.m[m_unclamp_workpiece] = {84};
    param.m[m_scrub_cycle_noreturn] = {85};
    param.m[m_revolver_cw_locked] = {86};
    param.m[m_revolver_cw_unlocked] = {87};
    param.m[m_spindle_on_axis_c_off] = {109};
    param.m[m_spindle_on_axis_c_on] = {110};
    param.m[m_alarm_on_stm_time_passed_on] = {124};
    param.m[m_alarm_on_stm_time_passed_off] = {125};
    param.m[m_profile_for_fixed_cycle] = {136};
    param.m[m_thread_tool_rpm_nowait] = {140};
    param.m[m_c_axis_use_locking] = {141};
    param.m[m_c_axis_unlock] = {146};
    param.m[m_c_axis_lock] = {147};
    param.m[m_revolver_allowed_without_axis_c_off] = {152};
    param.m[m_revolver_allowed_without_axis_c_on] = {153};
    param.m[m_nub_workmode_on] = {156};
    param.m[m_nub_workmode_off] = {157};
    param.m[m_feed_limit_manual_obey_on] = {160};
    param.m[m_feed_limit_manual_obey_off] = {161};
    param.m[m_rpm_limit_manual_obey_on] = {162};
    param.m[m_rpm_limit_manual_obey_off] = {163};
    param.m[m_single_block_obey_on] = {164};
    param.m[m_single_block_obey_off] = {165};
    param.m[m_nub_allowed_on_spindle_rotation_off] = {166};
    param.m[m_nub_allowed_on_spindle_rotation_on] = {167};
    param.m[m_chuck_opening_allowed_on_spindle_rotation_off] = {184};
    param.m[m_chuck_opening_allowed_on_spindle_rotation_on] = {185};
    //param.m[m_thread_cycle_phase_compensation_off] = {193};
    //param.m[m_thread_cycle_phase_compensation_on] = {194};
    // m_thread_cycle_save_manual_phase_compensation - on - 196
    // m_thread_cycle_save_manual_phase_compensation - on - 195
    // m_thread_cycle_save_manual_phase_compensation - delete - 197
    param.m[m_wedge_track_cycle_single_dimension] = {211};
    param.m[m_wedge_track_cycle_single_zigzag] = {212};
    param.m[m_wedge_track_cycle_specific_cutting_amount] = {213};
    param.m[m_wedge_track_cycle_equal_cutting_amount] = {214};
    param.m[m_multiop_rpm_min] = {241};
    param.m[m_multiop_rpm_max] = {242};

    return param;
}

}
