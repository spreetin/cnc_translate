#include <map>
#include "traubtx8h_definition.h"
#include "traub_auto_geometry_functions.h"

namespace NCParser {

MachineParameters TraubTX8H_definition::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings
    param.general.has_nose_radius_compenstion = true;
    param.maths.use_block_parenthesis = true;
    param.variables.variable_marker = {'L'};
    param.queueing.queueing_single_char = '!';
    param.subsystem.symbol = '$';
    param.subsystem.number = 2;

    // Axis
    param.parameter.subtypes['X'] = {1};
    param.parameter.subtypes['Y'] = {1};
    param.parameter.subtypes['Z'] = {1};
    param.parameter.subtypes['C'] = {1};
    param.parameter.is_diameter.insert('X');

    // Define changed default word meanings
    param.parameter.defaults['C'] = param_c_axis;
    param.parameter.defaults['D'] = param_undefined;
    param.parameter.defaults['L'] = param_variable;
    param.parameter.defaults['O'] = param_prg_name;

    // Variables
    param.subprograms.available_variables = {Token_TX8H_1, Token_TX8H_2};

    // Functions
    param.functions.unary = {{"SQR", f_square_root}, {"SIN", f_sine}, {"COS", f_cosine},
                             {"TAN", f_tangent}, {"ATN", f_arctangent}, {"RND", f_round},
                             {"FUP", f_ceil}, {"FIX", f_floor}, {"GOTO", f_jump}, {"IF", f_if},
                             {"ACOS", f_arccosine}, {"LN", f_ln}};
    param.functions.binary = {{"EQ", f_comparison_equal}, {"NE", f_comparison_not_equal},
                             {"GT", f_comparison_greater}, {"LT", f_comparison_lesser},
                             {"GE", f_comparison_greater_eq}, {"LE", f_comparsion_lesser_eq},
                              {"OR", f_or_b}, {"AND", f_and_b}, {"XOR", f_xor_b}, {"MOD", f_mod}};
    param.functions.block = {{"WHILE", {f_while, "WHILE", "DO"}}};

    // Unset default G-codes that are not valid
    param.g.erase(g_preload_registers);
    param.g.erase(g_thread_cutting_increasing_lead);
    param.g.erase(g_thread_cutting_decreasing_lead);

    // Set all defined G-codes
    param.g[g_rapid_positioning] = {0, gmode_motion,
            {'X', 'Z', 'U', 'W', 'F', 'S', 'T'}};
    param.g[g_linear_interpolation] = {1, gmode_motion,
            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'E', 'S', 'T'},
            {{'A', param_angle},
             {'E', param_transfer_feed}}};
    param.g[g_circular_interpolation_cw] = {2, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'P', 'Q',
             'R', 'D', 'C', 'H', 'F', 'E', 'S', 'T'},
            {{'I', param_circle_center_incr_x},
             {'K', param_circle_center_incr_z},
             {'P', param_circle_center_x},
             {'Q', param_circle_center_z},
             {'R', param_circle_radius},
             {'C', param_transfer_radius},
             {'D', param_transfer_chamfer},
             {'H', param_circle_transfer_point_bool},
             {'E', param_transfer_feed}}};
    param.g[g_circular_interpolation_ccw] = {3, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'P', 'Q',
             'R', 'D', 'C', 'H', 'F', 'E', 'S', 'T'},
            {{'I', param_circle_center_incr_x},
             {'K', param_circle_center_incr_z},
             {'P', param_circle_center_x},
             {'Q', param_circle_center_z},
             {'R', param_circle_radius},
             {'C', param_transfer_radius},
             {'D', param_transfer_chamfer},
             {'H', param_circle_transfer_point_bool},
             {'E', param_transfer_feed}}};
    param.g[g_dwell] = {4, gmode_nonmodal,
            {'X', 'U', 'S'},
            {{'X', param_dwell_time},
             {'U', param_dwell_time}}};
    param.g[g_linear_interpolation_with_collision_stop] = {8, gmode_motion,
            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'E', 'S', 'T'},
            {{'A', param_angle},
             {'E', param_transfer_feed}}};
    param.g[g_linear_interpolation_with_feed_reduction] = {9, gmode_motion,
            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'F', 'E', 'S', 'T'},
            {{'A', param_angle},
             {'D', param_transfer_chamfer},
             {'R', param_transfer_radius},
             {'E', param_transfer_feed}}};
    param.g[g_dimension_input_inch] = {20, gmode_nonmodal,
            {'S'}};
    param.g[g_dimension_input_metric] = {21, gmode_nonmodal,
            {'S'}};
    param.g[g_load_subprogram] = {22, gmode_nonmodal,
            {'A', 'P', 'Q', 'H'},
            {{'A', param_subprogram},
             {'P', param_starting_block},
             {'Q', param_ending_block},
             {'H', param_repetition}}};
    param.g[g_home_position_x] = {24, gmode_motion};
    param.g[g_home_position_z] = {25, gmode_motion};
    param.g[g_home_position_ordered_xz] = {26, gmode_motion};
    param.g[g_home_position_ordered_zx] = {27, gmode_motion};
    param.g[g_rapid_direct_to_reference] = {28, gmode_motion,
            {'X', 'Z', 'U', 'W', 'S'},
            {{'X', param_help_x},
             {'Z', param_help_z},
             {'U', param_help_u},
             {'W', param_help_w}}};
    param.g[g_thread_cutting_constant_lead] = {33, gmode_motion,
            {'X', 'Z', 'U', 'W', 'F', 'E'},
            {{'F', param_thread_lead_3decimals},
             {'E', param_thread_lead_5decimals}}};
    param.g[g_thread_cutting_variable_lead] = {34, gmode_motion,
            {'X', 'Z', 'U', 'W', 'F', 'E', 'K'},
            {{'F', param_thread_lead_3decimals},
             {'E', param_thread_lead_5decimals},
             {'K', param_thread_lead_change}}};
    param.g[g_cutter_compensation_auto] = {46, gmode_cutter_compensation};
    param.g[g_rapid_relative_to_reference] = {53, gmode_motion,
            {'X', 'Z', 'S'}};
    param.g[g_cutting_cycle_convex_tool_x] = {70, gmode_motion,
            {'A', 'P', 'Q', 'I', 'K', 'D', 'J', 'F', 'S'},
            {{'A', param_subprogram},
             {'P', param_starting_block},
             {'Q', param_ending_block},
             {'I', param_margin_x},
             {'K', param_margin_z},
             {'D', param_margin_cutting_depth},
             {'J', param_tool_angle}}};
    param.g[g_cutting_cycle_concave_tool_x] = {71, gmode_motion,
            {'A', 'P', 'Q', 'I', 'K', 'D', 'F', 'E', 'S'},
            {{'A', param_subprogram},
             {'P', param_starting_block},
             {'Q', param_ending_block},
             {'I', param_margin_x},
             {'K', param_margin_z},
             {'D', param_margin_cutting_depth},
             {'E', param_feed_downwards}}};
    param.g[g_cutting_cycle_z] = {72, gmode_motion,
            {'A', 'P', 'Q', 'I', 'K', 'D', 'F', 'E', 'S'},
            {{'A', param_subprogram},
             {'P', param_starting_block},
             {'Q', param_ending_block},
             {'I', param_margin_x},
             {'K', param_margin_z},
             {'D', param_margin_cutting_depth},
             {'E', param_feed_downwards}}};
    param.g[g_cutting_cycle_same_contour] = {73, gmode_motion,
            {'A', 'P', 'Q', 'U', 'W', 'I', 'K', 'D', 'F', 'E', 'S'},
            {{'A', param_subprogram},
             {'P', param_starting_block},
             {'Q', param_ending_block},
             {'I', param_margin_x},
             {'K', param_margin_z},
             {'D', param_margin_cutting_depth},
             {'E', param_feed_downwards},
             {'I', param_margin_start_x},
             {'K', param_margin_start_z}}};
    param.g[g_cutting_cycle_straight_with_fallback] = {74, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'D', 'F'},
            {{'K', param_distance_before_fallback},
             {'D', param_fallback_distance},
             {'I', param_margin_cutting_depth}}};
    param.g[g_cutting_cycle_straight_with_fallback_z] = {75, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'D', 'F'},
            {{'K', param_distance_before_fallback},
             {'D', param_fallback_distance},
             {'I', param_margin_cutting_depth}}};
    param.g[g_thread_cutting_cycle] = {76, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'J', 'K', 'H', 'F', 'E', 'A', 'D'},
            {{'I', param_conical_amount},
             {'J', param_conical_angle},
             {'K', param_thread_depth},
             {'H', param_num_cutters},
             {'F', param_thread_lead_3decimals},
             {'E', param_thread_lead_5decimals},
             {'A', param_angle},
             {'D', param_depth_last_cutter}}};
    param.g[g_bevel_cycle_towards_spindle] = {77, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'F'},
            {{'I', param_help_x},
             {'K', param_help_z}}};
    // G78
    param.g[g_bevel_cycle_against_spindle] = {79, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'K', 'F'},
            {{'I', param_help_x},
             {'K', param_help_z}}};
    param.g[g_cycle_repetition] = {81, gmode_motion,
            {'U', 'W', 'H'},
            {{'H', param_repetition}}}; // Only for 77,78,79
    // G82
    param.g[g_drill_cycle_step_number] = {83, gmode_motion,
            {'X', 'Z', 'U', 'W', 'D', 'H', 'F'},
            {{'D', param_distance_before_fallback},
             {'H', param_repetition}}};
    param.g[g_drill_cycle_step_distance] = {84, gmode_motion,
            {'X', 'Z', 'U', 'W', 'D', 'I', 'K', 'A', 'Q', 'R', 'F'},
            {{'D', param_rapid_distance},
             {'I', param_distance_before_fallback},
             {'K', param_step_distance},
             {'A', param_margin},
             {'Q', param_dwell_time},
             {'R', param_secondary_dwell_time}}};
    param.g[g_thread_cutting_end_face] = {86, gmode_motion,
            {'X', 'Z', 'U', 'W', 'I', 'J', 'K', 'H', 'F', 'E', 'A', 'D'},
            {{'I', param_conical_amount},
             {'J', param_conical_angle},
             {'K', param_thread_depth},
             {'H', param_num_cutters},
             {'F', param_thread_lead_3decimals},
             {'E', param_thread_lead_5decimals},
             {'A', param_angle},
             {'D', param_depth_last_cutter}}};
    param.g[g_rpm_limit] = {92, -1,
            {'S', 'Q'},
            {{'S', param_rpm_max},
             {'Q', param_rpm_min}}};
    param.g[g_feed_per_minute] = {94, gmode_feed_rate,
            {'S'},
            {{'S', param_rpm_or_feed}}};
    param.g[g_feed_per_revolution] = {95, gmode_feed_rate,
            {'S'},
            {{'S', param_rpm_or_feed}}};
    param.g[g_constant_surface_speed] = {96, gmode_spindle_speed,
            {'V', 'T'},
            {{'V', param_feed}}};
    param.g[g_revolutions_per_minute] = {97, gmode_spindle_speed,
            {'S', 'V', 'X', 'T'},
            {{'X', param_diameter},
             {'V', param_feed}}};
    param.g[g_feed_correction] = {98, gmode_feed_rate,
            {'S'},
            {{'S', param_rpm}}};
    param.g[g_feed_correction_off] = {99, gmode_feed_rate,
            {'S'},
            {{'S', param_rpm}}};
    // G201-299 makros
    // G387
    param.g[g_auto_geometry_function] = {std::set<int>{101, 102, 105, 106}};

    // Define M-codes
    param.m[m_activate_c_axis] = {17};
    param.m[m_deactivate_c_axis] = {18};
    param.m[m_activate_c_axis_opposing_spindle] = {217};
    param.m[m_deactivate_c_axis_opposing_spindle] = {218};

    // Define functions
    param.auto_geometry_functions = {
        {101, gmode_motion, {}, {}, std::shared_ptr<auto_geometry_function>{new traub_function_g101()}},
        {102, gmode_motion, {}, {}, std::shared_ptr<auto_geometry_function>{new traub_function_g102()}},
        {105, gmode_motion, {}, {}, std::shared_ptr<auto_geometry_function>{new traub_function_g105()}},
        {106, gmode_motion, {}, {}, std::shared_ptr<auto_geometry_function>{new traub_function_g106()}},
    };
    return param;
}

};
