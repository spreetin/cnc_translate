#ifndef PARAMETER_DEFINITIONS_H
#define PARAMETER_DEFINITIONS_H

#include <map>
#include <variant>
#include <set>
#include <memory>
#include "auto_geometry_function.h"

namespace NCParser {

struct variable_group {
    char letter;
    int first;
    int last;
};

enum ParameterType {
    param_undefined,

    // Undefined parameters
    param_a = 'A',
    param_b = 'B',
    param_c = 'C',
    param_d = 'D',
    param_e = 'E',
    param_f = 'F',
    param_h = 'H',
    param_i = 'I',
    param_j = 'J',
    param_k = 'K',
    param_l = 'L',
    param_o = 'O',
    param_p = 'P',
    param_q = 'Q',
    param_r = 'R',
    param_s = 'S',
    param_t = 'T',
    param_u = 'U',
    param_v = 'V',
    param_w = 'W',

    // Default defined meanings
    param_angular_dimension_x,
    param_angular_dimension_y,
    param_angular_dimension_z,
    param_tool_compensation_data,
    param_feed_secondary,
    param_feed,
    param_incremental_x,
    param_incremental_y,
    param_incremental_z,
    param_tertiary_x,
    param_tertiary_y,
    param_tertiary_z,
    param_rpm,
    param_tool_selection,
    param_secondary_x,
    param_secondary_y,
    param_secondary_z,
    param_x,
    param_y,
    param_z,

    // Other meaning
    param_transfer_feed,
    param_transfer_radius,
    param_transfer_chamfer,
    param_angle,
    param_diameter,
    param_variable,
    param_rpm_max,
    param_rpm_min,
    param_rpm_or_feed,
    param_prg_name,
    param_help_x,
    param_help_z,
    param_help_u,
    param_help_w,
    param_circle_center_x,
    param_circle_center_z,
    param_circle_center_incr_x,
    param_circle_center_incr_z,
    param_circle_radius,
    param_circle_transfer_point_bool,
    param_dwell_time,
    param_secondary_dwell_time,
    param_subprogram,
    param_starting_block,
    param_ending_block,
    param_margin,
    param_margin_x,
    param_margin_z,
    param_margin_cutting_depth,
    param_tool_angle,
    param_feed_downwards,
    param_margin_start_x,
    param_margin_start_z,
    param_distance_before_fallback,
    param_fallback_distance,
    param_repetition,
    param_rapid_distance,
    param_step_distance,
    param_thread_lead,
    param_thread_lead_5decimals,
    param_thread_lead_3decimals,
    param_thread_lead_change,
    param_conical_amount,
    param_conical_angle,
    param_thread_depth,
    param_num_cutters,
    param_depth_last_cutter,
    param_c_axis,
};

enum GModalGroups {
    gmode_nonmodal = 0,
    gmode_motion = 1,
    gmode_plane_selection = 2,
    gmode_distance = 3,
    gmode_arc_disance = 4,
    gmode_feed_rate = 5,
    gmode_units = 6,
    gmode_cutter_compensation = 7,
    gmode_tool_length_offset = 8,
    gmode_return_mode = 10,
    gmode_work_coordinate_system = 12,
    gmode_control_mode = 13,
    gmode_spindle_speed = 14,
    gmode_lathe_diameter_mode = 15,
};

struct g_word_definition {
    std::variant<int,std::set<int>> word = -1;
    int modal = -1;
    std::set<char> parameters;
    std::map<char,ParameterType> parameter_types;
    std::shared_ptr<auto_geometry_function> func;
};

enum MModalGroups {
    mmode_stopping = 4,
    mmode_io_onoff = 5,
    mmode_tool_change = 6,
    mmode_spindle = 7,
    mmode_coolant = 8,
    mmode_override = 9,
    mmode_user_defined = 10
};

struct m_word_definition {
    int num = -1;
};

enum Functions {
    f_multiply,
    f_divide,
    f_xor,
    f_mod,
    f_ln,
    f_exponent,
    f_abs,
    f_sine,
    f_cosine,
    f_arccosine,
    f_tangent,
    f_arctangent,
    f_square_root,
    f_round,
    f_ceil,
    f_floor,
    f_jump,
    f_if,
    f_or,
    f_and,
    f_while,
    f_for,
    f_comparison_equal,
    f_comparison_not_equal,
    f_comparison_greater,
    f_comparison_lesser,
    f_comparison_greater_eq,
    f_comparsion_lesser_eq,
};

struct block_function {
    int type;
    std::string start_token;
    std::string end_token;
};

};

#endif // PARAMETER_DEFINITIONS_H
