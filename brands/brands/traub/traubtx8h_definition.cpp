#include <map>
#include "traubtx8h_definition.h"
#include "traub_auto_geometry_functions.h"

namespace NCParser {

void TraubTX8H_definition::updateData(std::shared_ptr<std::map<int,GCode>> gCodes,
                                 std::shared_ptr<std::map<int,MCode>> mCodes,
                                 std::shared_ptr<std::map<int, FixedCycleDefinition> > fixedCycleDefinition)
{

}

MachineParameters TraubTX8H_definition::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings
    param.general.has_nose_radius_compenstion = true;
    param.maths.use_block_parenthesis = true;
    param.variables.variable_marker = {'L'};

    // Define changed default word meanings
    param.parameter_defaults['C'] = param_c_axis;
    param.parameter_defaults['D'] = param_undefined;
    param.parameter_defaults['L'] = param_variable;
    param.parameter_defaults['O'] = param_prg_name;

    // Variables
    param.subprograms.available_variables = {{'L', 1, 50},{'L', 101, 150}};

    // Functions
    param.functions.unary = {{"SQR", f_square_root}, {"SIN", f_sine}, {"COS", f_cosine},
                             {"TAN", f_tangent}, {"ATN", f_arctangent}, {"RND", f_round},
                             {"FUP", f_ceil}, {"FIX", f_floor}, {"GOTO", f_jump}, {"IF", f_if},
                             {"ACOS", f_arccosine}, {"LN", f_ln}};
    param.functions.binary = {{"EQ", f_comparison_equal}, {"NE", f_comparison_not_equal},
                             {"GT", f_comparison_greater}, {"LT", f_comparison_lesser},
                             {"GE", f_comparison_greater_eq}, {"LE", f_comparsion_lesser_eq},
                              {"OR", f_or}, {"AND", f_and}, {"XOR", f_xor}, {"MOD", f_mod}};
    param.functions.block = {{"WHILE", {f_while, "WHILE", "DO"}}};

    // Unset default G-codes that are not valid
    param.g.erase(preload_registers);
    param.g.erase(fixed_cycle);
    param.g.erase(thread_cutting_increasing_lead);
    param.g.erase(thread_cutting_decreasing_lead);

    // Set all defined G-codes
    param.g[rapid_positioning] = {0, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'F', 'S', 'T'}};
    param.g[linear_interpolation] = {1, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'E', 'S', 'T'},
                            {{'A', param_angle}, {'E', param_transfer_feed}}};
    param.g[circular_interpolation_cw] = {2, gmode_motion,
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
    param.g[circular_interpolation_ccw] = {3, gmode_motion,
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
    param.g[dwell] = {4, gmode_nonmodal, {'X', 'U', 'S'},
                     {{'X', param_dwell_time}, {'U', param_dwell_time}}};
    param.g[lin_interpol_with_collision_stop] = {8, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'E', 'S', 'T'},
                            {{'A', param_angle}, {'E', param_transfer_feed}}};
    param.g[linear_interpolation_with_feed_reduction] = {9, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'A', 'D', 'R', 'F', 'E', 'S', 'T'},
                            {{'A', param_angle}, {'D', param_transfer_chamfer},
                            {'R', param_transfer_radius}, {'E', param_transfer_feed}}};
    param.g[dimension_input_inch] = {20, gmode_nonmodal, {'S'}};
    param.g[dimension_input_metric] = {21, gmode_nonmodal, {'S'}};
    param.g[load_subprogram] = {22, gmode_nonmodal, {'A', 'P', 'Q', 'H'},
                            {{'A', param_subprogram}, {'P', param_starting_block},
                            {'Q', param_ending_block}, {'H', param_repetition}}};
    param.g[home_position_x] = {24, gmode_motion};
    param.g[home_position_z] = {25, gmode_motion};
    param.g[home_position_ordered_xz] = {26, gmode_motion};
    param.g[home_position_ordered_zx] = {27, gmode_motion};
    param.g[rapid_direct_to_reference] = {28, gmode_motion, {'X', 'Z', 'U', 'W', 'S'},
                            {{'X', param_help_x}, {'Z', param_help_z},
                            {'U', param_help_u}, {'W', param_help_w}}};
    param.g[thread_cutting_constant_lead] = {33, gmode_motion, {'X', 'Z', 'U', 'W', 'F', 'E'},
                            {{'F', param_thread_lead_3decimals},
                            {'E', param_thread_lead_5decimals}}};
    param.g[thread_cutting_variable_lead] = {34, gmode_motion, {'X', 'Z', 'U', 'W', 'F', 'E', 'K'},
                                            {{'F', param_thread_lead_3decimals},
                                             {'E', param_thread_lead_5decimals},
                                             {'K', param_thread_lead_change}}};
    param.g[cutter_compensation_auto] = {46, gmode_cutter_compensation};
    param.g[rapid_relative_to_reference] = {53, gmode_motion, {'X', 'Z', 'S'}};
    param.g[cutting_cycle_convex_tool_x] = {70, gmode_motion,
                            {'A', 'P', 'Q', 'I', 'K', 'D', 'J', 'F', 'S'},
                            {{'A', param_subprogram}, {'P', param_starting_block},
                            {'Q', param_ending_block}, {'I', param_margin_x},
                            {'K', param_margin_z}, {'D', param_margin_cutting_depth},
                            {'J', param_tool_angle}}};
    param.g[cutting_cycle_concave_tool_x] = {71, gmode_motion,
                            {'A', 'P', 'Q', 'I', 'K', 'D', 'F', 'E', 'S'},
                            {{'A', param_subprogram}, {'P', param_starting_block},
                             {'Q', param_ending_block}, {'I', param_margin_x},
                             {'K', param_margin_z}, {'D', param_margin_cutting_depth},
                             {'E', param_feed_downwards}}};
    param.g[cutting_cycle_z] = {72, gmode_motion,
                           {'A', 'P', 'Q', 'I', 'K', 'D', 'F', 'E', 'S'},
                           {{'A', param_subprogram}, {'P', param_starting_block},
                            {'Q', param_ending_block}, {'I', param_margin_x},
                            {'K', param_margin_z}, {'D', param_margin_cutting_depth},
                            {'E', param_feed_downwards}}};
    param.g[cutting_cycle_same_contour] = {73, gmode_motion,
                          {'A', 'P', 'Q', 'U', 'W', 'I', 'K', 'D', 'F', 'E', 'S'},
                          {{'A', param_subprogram}, {'P', param_starting_block},
                           {'Q', param_ending_block}, {'I', param_margin_x},
                           {'K', param_margin_z}, {'D', param_margin_cutting_depth},
                           {'E', param_feed_downwards}, {'I', param_margin_start_x},
                           {'K', param_margin_start_z}}};
    param.g[cutting_cycle_straight_with_fallback] = {74, gmode_motion,
                           {'X', 'Z', 'U', 'W', 'I', 'K', 'D', 'F'},
                           {{'K', param_distance_before_fallback}, {'D', param_fallback_distance},
                            {'I', param_margin_cutting_depth}}};
    param.g[cutting_cycle_straight_with_fallback_z] = {75, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'I', 'K', 'D', 'F'},
                            {{'K', param_distance_before_fallback}, {'D', param_fallback_distance},
                             {'I', param_margin_cutting_depth}}};
    param.g[thread_cutting_cycle] = {76, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'I', 'J', 'K', 'H', 'F', 'E', 'A', 'D'},
                            {{'I', param_conical_amount}, {'J', param_conical_angle},
                            {'K', param_thread_depth}, {'H', param_num_cutters},
                            {'F', param_thread_lead_3decimals}, {'E', param_thread_lead_5decimals},
                            {'A', param_angle}, {'D', param_depth_last_cutter}}};
    param.g[bevel_cycle_towards_spindle] = {77, gmode_motion, {'X', 'Z', 'U', 'W', 'I', 'K', 'F'},
                            {{'I', param_help_x}, {'K', param_help_z}}};
    // G78
    param.g[bevel_cycle_against_spindle] = {79, gmode_motion, {'X', 'Z', 'U', 'W', 'I', 'K', 'F'},
                            {{'I', param_help_x}, {'K', param_help_z}}};
    param.g[cycle_repetition] = {81, gmode_motion, {'U', 'W', 'H'}, {{'H', param_repetition}}}; // Only for 77,78,79
    // G82
    param.g[drill_cycle_step_number] = {83, gmode_motion, {'X', 'Z', 'U', 'W', 'D', 'H', 'F'},
                            {{'D', param_distance_before_fallback},{'H', param_repetition}}};
    param.g[drill_cycle_step_distance] = {84, gmode_motion,
                            {'X', 'Z', 'U', 'W', 'D', 'I', 'K', 'A', 'Q', 'R', 'F'},
                            {{'D', param_rapid_distance}, {'I', param_distance_before_fallback},
                            {'K', param_step_distance}, {'A', param_margin},
                            {'Q', param_dwell_time}, {'R', param_secondary_dwell_time}}};
    // param.g[??? = {86} -- thread cycle on plane
    param.g[rpm_limit] = {92, -1, {'S', 'Q'},
                            {{'S', param_rpm_max}, {'Q', param_rpm_min}}};
    param.g[feed_per_minute] = {94, gmode_feed_rate, {'S'},
                            {{'S', param_rpm_or_feed}}};
    param.g[feed_per_revolution] = {95, gmode_feed_rate, {'S'},
                            {{'S', param_rpm_or_feed}}};
    param.g[constant_surface_speed] = {96, gmode_spindle_speed, {'V', 'T'},
                            {{'V', param_feed}}};
    param.g[revolutions_per_minute] = {97, gmode_spindle_speed, {'S', 'V', 'X', 'T'},
                            {{'X', param_diameter}, {'V', param_feed}}};
    param.g[feed_correction] = {98, gmode_feed_rate, {'S'},
                            {{'S', param_rpm}}};
    param.g[feed_correction_off] = {99, gmode_feed_rate, {'S'},
                            {{'S', param_rpm}}};
    // G201-299 makros
    // G387

    // Define M-codes
    param.m[activate_c_axis] = {17};
    param.m[deactivate_c_axis] = {18};
    param.m[activate_c_axis_opposing_spindle] = {217};
    param.m[deactivate_c_axis_opposing_spindle] = {218};

    //param.g[auto_geometry_functions = std::map<int,g_word_definition>{{101, {101, gmode_motion, {}, {}, new traub_function_g101()}}};
    return param;
}

};
