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
        char hexadecimal = '\0';
    } maths;

    struct subsystems {
        char symbol = '\0';
        int number = 1;
    } subsystem;

    struct variables {
        std::set<char> variable_marker;
        std::vector<RegexToken> available_variables;
    } variables;

    struct subprograms {
        std::vector<RegexToken> available_variables;
    } subprograms;

    struct functions {
        std::map<std::string,int> unary;
        std::map<std::string,int> binary;
        std::map<std::string,int> coordinate;
        std::map<std::string,block_function> block;
    } functions;

    struct queueing {
        bool has_queueing = true;
        char queueing_single_char = '\0';
        char queueing_numbered_single_char = '\0';
        std::string queueing_string;
        std::string queueing_numbered_string;
    } queueing;

    struct tool_selection {
        std::map<std::string,std::vector<std::pair<ParameterType,int>>> parts = {
            {"T", {{param_tool_selection_tool, 2}, {param_tool_selection_tooldata, 2}}},
        };
    } tool_selection;

    std::map<int,SpindleTypes> spindles;

    struct parameter {
        std::map<char,std::vector<int>> subtypes;

        std::map<char,ParameterType> defaults = {
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

        std::set<char> is_diameter;

        std::map<std::string,ParameterType> defaults_multiletter;

        std::set<char> always_useable;
    } parameter;



    std::map<int, g_word_definition> g = {
        /*{g_rapid_positioning, {0, gmode_motion, {'X', 'Y', 'Z', 'U', 'V', 'W', 'A', 'B', 'C', 'F', 'S', 'T'}}},
        {g_linear_interpolation, {1, gmode_motion}},
        {g_circular_interpolation_cw, {2, gmode_motion}},
        {g_circular_interpolation_ccw, {3, gmode_motion}},
        {g_dwell, {4, gmode_nonmodal, {'F', 'P', 'X', 'S'}}},
        {g_parabolic_interpolation, {6}},
        {g_exact_stop_block, {9, gmode_control_mode}},
        {g_xy_plane_selection, {17, gmode_plane_selection, {}}},
        {g_zx_plane_selection, {18, gmode_plane_selection, {}}},
        {g_zy_plane_selection, {19, gmode_plane_selection, {}}},
        {g_thread_cutting_constant_lead, {33}},
        {g_thread_cutting_increasing_lead, {34}},
        {g_thread_cutting_decreasing_lead, {35}},
        {g_cutter_compensation_cancel, {40, gmode_cutter_compensation}},
        {g_cutter_compensation_left, {41, gmode_cutter_compensation}},
        {g_cutter_compensation_right, {42, gmode_cutter_compensation}},
        {g_tool_offset_positive, {43, gmode_tool_length_offset}},
        {g_tool_offset_negative, {44, gmode_tool_length_offset}},
        {g_dimension_shift_cancel, {53, gmode_nonmodal}},
        {g_zero_shift, {std::set<int>{54, 55, 56, 57, 58, 59}, -1}},
        {g_exact_stop, {60, gmode_control_mode}},
        {g_tapping, {63}},
        {g_continuous_path_mode, {64, gmode_control_mode}},
        {g_dimension_input_inch, {70, gmode_units, {}}},
        {g_dimension_input_metric, {71, gmode_units, {}}},
        {g_home_position, {74, gmode_nonmodal, {'X', 'Y', 'Z'}}},
        {g_fixed_cycle_cancel, {80}},
        {g_absolute_dimension, {90, gmode_distance}},
        {g_incremental_dimension, {91, gmode_distance}},
        {g_preload_registers, {92, gmode_nonmodal}},
        {g_inverse_time, {93, gmode_feed_rate}},
        {g_feed_per_minute, {94, gmode_feed_rate}},
        {g_feed_per_revolution, {95, gmode_feed_rate}},
        {g_constant_surface_speed, {96, gmode_spindle_speed}},
        {g_revolutions_per_minute, {97, gmode_spindle_speed}},
        {g_coordinate_system_set, {-1, -1, {'P', 'X', 'Y', 'Z', 'A', 'B', 'C'}}}*/
    };

    std::vector<g_word_definition> auto_geometry_functions;

    std::map<int, m_word_definition> m;

};

};

#endif // PARAMETERS_H
