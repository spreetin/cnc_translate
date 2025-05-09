#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <set>
#include <vector>
#include <map>
#include "parameter_definitions.h"
#include "../tokens/g_codes.h"
#include "../tokens/m_codes.h"

namespace NCParser {

struct MachineParameters {

    struct general {
        bool has_nose_radius_compenstion = false;
        int max_g_per_block = 2;
    } general;

    struct comments {
        bool use_parenthesis = true;
        bool use_semicolon = false;
    } comments;

    struct maths {
        bool use_block_parenthesis = false;
    } maths;

    struct variables {
        std::set<char> variable_marker;
        std::vector<variable_group> available_variables;
    } variables;

    struct subprograms {
        std::vector<variable_group> available_variables;
    } subprograms;

    struct functions {
        std::map<std::string,int> unary;
        std::map<std::string,int> binary;
        std::map<std::string,block_function> block;
    } functions;

    std::map<char,ParameterType> parameter_defaults = {
        {'A', param_angular_dimension_x},
        {'B', param_angular_dimension_y},
        {'C', param_angular_dimension_z},
        {'D', param_tool_compensation_data},
        {'E', param_feed_secondary},
        {'F', param_feed},
        {'H', param_undefined},
        {'I', param_incremental_x},
        {'J', param_incremental_y},
        {'K', param_incremental_z},
        {'L', param_undefined},
        {'O', param_undefined},
        {'P', param_tertiary_x},
        {'Q', param_tertiary_y},
        {'R', param_tertiary_z},
        {'S', param_rpm},
        {'T', param_tool_selection},
        {'U', param_secondary_x},
        {'V', param_secondary_y},
        {'W', param_secondary_z},
        {'X', param_x},
        {'Y', param_y},
        {'Z', param_z}
    };

    std::map<int, g_word_definition> g = {
        {rapid_positioning, {0, gmode_motion, {'X', 'Y', 'Z', 'U', 'V', 'W', 'A', 'B', 'C', 'F', 'S', 'T'}}},
        {linear_interpolation, {1, gmode_motion}},
        {circular_interpolation_cw, {2, gmode_motion}},
        {circular_interpolation_ccw, {3, gmode_motion}},
        {dwell, {4, gmode_nonmodal, {'F', 'P', 'X', 'S'}}},
        {parabolic_interpolation, {6}},
        {exact_stop_block, {9, gmode_control_mode}},
        {xy_plane_selection, {17, gmode_plane_selection, {}}},
        {zx_plane_selection, {18, gmode_plane_selection, {}}},
        {zy_plane_selection, {19, gmode_plane_selection, {}}},
        {thread_cutting_constant_lead, {33}},
        {thread_cutting_increasing_lead, {34}},
        {thread_cutting_decreasing_lead, {35}},
        {cutter_compensation_cancel, {40, gmode_cutter_compensation}},
        {cutter_compensation_left, {41, gmode_cutter_compensation}},
        {cutter_compensation_right, {42, gmode_cutter_compensation}},
        {tool_offset_positive, {43, gmode_tool_length_offset}},
        {tool_offset_negative, {44, gmode_tool_length_offset}},
        {dimension_shift_cancel, {53, gmode_nonmodal}},
        {zero_shift, {std::set<int>{54, 55, 56, 57, 58, 59}, -1}},
        {exact_stop, {60, gmode_control_mode}},
        {tapping, {63}},
        {continuous_path_mode, {64, gmode_control_mode}},
        {dimension_input_inch, {70, gmode_units, {}}},
        {dimension_input_metric, {71, gmode_units, {}}},
        {home_position, {74, gmode_nonmodal, {'X', 'Y', 'Z'}}},
        {fixed_cycle_cancel, {80}},
        {fixed_cycle, {std::set<int>{81, 82, 83, 84, 85, 86, 87, 88, 89}, gmode_motion}},
        {absolute_dimension, {90, gmode_distance}},
        {incremental_dimension, {91, gmode_distance}},
        {preload_registers, {92, gmode_nonmodal}},
        {inverse_time, {93, gmode_feed_rate}},
        {feed_per_minute, {94, gmode_feed_rate}},
        {feed_per_revolution, {95, gmode_feed_rate}},
        {constant_surface_speed, {96, gmode_spindle_speed}},
        {revolutions_per_minute, {97, gmode_spindle_speed}},
        {coordinate_system_set, {-1, -1, {'P', 'X', 'Y', 'Z', 'A', 'B', 'C'}}},
        {initial_level_return_fixed, {}},
        {r_point_return_fixed, {}},
        {rpm_limit, {}},
        {feed_correction, {}},
        {feed_correction_off, {}},
        {home_position_x, {}},
        {home_position_z, {}},
        {home_position_ordered_xz, {}},
        {home_position_ordered_zx, {}},
        {rapid_direct_to_reference, {}},
        {rapid_relative_to_reference, {}},
        {linear_interpolation_with_feed_reduction, {}},
        {cutter_compensation_auto, {}},
        {lin_interpol_with_collision_stop, {}},
        {cutting_cycle_convex_tool_x, {}},
        {cutting_cycle_concave_tool_x, {}},
        {cutting_cycle_z, {}},
        {cutting_cycle_same_contour, {}},
        {cutting_cycle_straight_with_fallback, {}},
        {cutting_cycle_straight_with_fallback_z, {}},
        {bevel_cycle_towards_spindle, {}},
        {bevel_cycle_against_spindle, {}},
        {cycle_repetition, {}},
        {drill_cycle_step_number, {}},
        {drill_cycle_step_distance, {}},
        {thread_cutting_variable_lead, {}},
        {thread_cutting_cycle, {}},
        {load_subprogram, {}},
        {auto_geometry_function, {}}
    };

    std::map<int,g_word_definition> auto_geometry_functions;

    std::map<int, m_word_definition> m = {
        {program_stop, {0}},
        {optional_stop, {1}},
        {end_of_program, {2}},
        {spindle_cw, {3}},
        {spindle_ccw, {4}},
        {spindle_off, {5}},
        {tool_change, {6}},
        {clamp_workpiece, {10}},
        {unclamp_workpiece, {11}},
        {end_of_data, {30}},
        {cancel_m49, {48}},
        {bypass_override, {49}},
        {workpiece_change, {60}},
        {activate_c_axis, {}},
        {deactivate_c_axis, {}},
        {activate_c_axis_opposing_spindle, {}},
        {deactivate_c_axis_opposing_spindle, {}},
    };

};

};

#endif // PARAMETERS_H
