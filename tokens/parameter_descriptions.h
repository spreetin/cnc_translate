#ifndef PARAMETER_DESCRIPTIONS_H
#define PARAMETER_DESCRIPTIONS_H

#include <string_view>
#include "../brands/parameter_definitions.h"

namespace NCParser {

static constexpr std::string_view parameter_name(ParameterType code){
    switch (code){
    case param_undefined:
        return "Undefined parameter";
    case param_a:
        return "Param character A";
    case param_b:
        return "Param character B";
    case param_c:
        return "Param character C";
    case param_d:
        return "Param character D";
    case param_e:
        return "Param character E";
    case param_f:
        return "Param character F";
    case param_h:
        return "Param character H";
    case param_i:
        return "Param character I";
    case param_j:
        return "Param character J";
    case param_k:
        return "Param character K";
    case param_l:
        return "Param character L";
    case param_o:
        return "Param character O";
    case param_p:
        return "Param character P";
    case param_q:
        return "Param character Q";
    case param_r:
        return "Param character R";
    case param_s:
        return "Param character S";
    case param_t:
        return "Param character T";
    case param_u:
        return "Param character U";
    case param_v:
        return "Param character V";
    case param_w:
        return "Param character W";
    case param_angular_dimension_x:
        return "Angular dimensions in X";
    case param_angular_dimension_y:
        return "Angular dimensions in Y";
    case param_angular_dimension_z:
        return "Angular dimensions in Z";
    case param_tool_compensation_data:
        return "Tool compensation data";
    case param_feed_secondary:
        return "Secondary feed value";
    case param_feed:
        return "Feed value";
    case param_incremental_x:
        return "Incremental X value";
    case param_incremental_y:
        return "Incremental Y value";
    case param_incremental_z:
        return "Incremental Z value";
    case param_tertiary_x:
        return "Tertiary X value";
    case param_tertiary_y:
        return "Tertiary Y value";
    case param_tertiary_z:
        return "Tertiary Z value";
    case param_rpm:
        return "Spindle revolutions";
    case param_tool_selection:
        return "Tool selection";
    case param_secondary_x:
        return "Secondary X value";
    case param_secondary_y:
        return "Secondary Y value";
    case param_secondary_z:
        return "Secondary Z value";
    case param_x:
        return "X value";
    case param_y:
        return "Y value";
    case param_z:
        return "Z value";
    case param_angle:
        return "Angle";
    case param_c_axis:
        return "C axis value";
    case param_c_axis_rotation:
        return "C axis rotation";
    case param_c_axis_rpm:
        return "C axis RPM value";
    case param_circle_center_incr_x:
        return "Incremental circle center X coordinate";
    case param_circle_center_incr_z:
        return "Incremental circle center Z coordinate";
    case param_circle_center_x:
        return "Circle center X coordinate";
    case param_circle_center_z:
        return "Circle center Z coordinate";
    case param_circle_radius:
        return "Circle radius";
    case param_circle_transfer_point_bool:
        return "Use transfer point for circle";
    case param_conical_amount:
        return "Conicity amount";
    case param_conical_angle:
        return "Conicity angle";
    case param_depth_last_cutter:
        return "Depth for last cutter";
    case param_diameter:
        return "Diameter";
    case param_distance_before_fallback:
        return "Distance before fallback";
    case param_drilling_depth:
        return "Drilling deapth";
    case param_drilling_depth_tool_removal:
        return "";
    case param_dwell_time:
        return "Dwell time";
    case param_ending_block:
        return "End block";
    case param_fallback_distance:
        return "Fallback distance";
    case param_feed_downwards:
        return "Feed value downwards";
    case param_help_u:
        return "";
    case param_help_w:
        return "";
    case param_help_x:
        return "";
    case param_help_z:
        return "";
    case param_margin:
        return "Margin";
    case param_margin_cutting_depth:
        return "";
    case param_margin_start_x:
        return "Margin in X for start";
    case param_margin_start_z:
        return "Margin in Z for start";
    case param_margin_x:
        return "Margin in X";
    case param_margin_z:
        return "Margin in Z";
    case param_milling_depth:
        return "Milling depth";
    case param_milling_depth_tool_removal:
        return "";
    case param_num_cutters:
        return "Number of cutters";
    case param_prg_name:
        return "Program name";
    case param_queueing:
        return "Queueing marker";
    case param_queueing_numbered:
        return "Numbered queueing marker";
    case param_rapid_distance:
        return "Distance for rapid positioning";
    case param_rapid_feed:
        return "Feed rate for rapid positioning";
    case param_repetition:
        return "Repetitions";
    case param_rpm_max:
        return "Maximum RPM value";
    case param_rpm_min:
        return "Minimum RPM value";
    case param_rpm_or_feed:
        return "RPM or feed value";
    case param_secondary_dwell_time:
        return "Secondary dwell time";
    case param_set_c_value_on_current:
        return "";
    case param_set_x_value_on_current:
        return "";
    case param_set_z_value_on_current:
        return "";
    case param_starting_block:
        return "Starting block";
    case param_step_distance:
        return "";
    case param_subprogram:
        return "Subprogram";
    case param_thread_champfer_end_length:
        return "";
    case param_thread_depth:
        return "Thread depth";
    case param_thread_depth_first_cut:
        return "Thread depth on first cut";
    case param_thread_lead:
        return "Thread lead";
    case param_thread_lead_3decimals:
        return "Thread lead, 3 decimals";
    case param_thread_lead_5decimals:
        return "Thread lead, 5 decimals";
    case param_thread_lead_change:
        return "";
    case param_thread_lead_count:
        return "Thread lead count";
    case param_thread_phase_difference:
        return "Phase difference for thread";
    case param_thread_start_offset:
        return "Offset for thread start";
    case param_tool_angle:
        return "Tool angle";
    case param_tool_preparation:
        return "";
    case param_tool_rpm:
        return "Tool RPM value";
    case param_tool_selection_nrc:
        return "";
    case param_tool_selection_offset:
        return "";
    case param_tool_selection_tool:
        return "";
    case param_tool_selection_tooldata:
        return "";
    case param_transfer_chamfer:
        return "Chamfer on transfer";
    case param_transfer_feed:
        return "Feed on transfer";
    case param_transfer_radius:
        return "Radius on transfer";
    case param_variable:
        return "Variable";
    case parma_tool_selection_pos:
        return "Tool selection position";
    case param_x_start:
        return "Starting value in X";
    case param_opposing_spindle_approach_axis:
        return "Axis for opposing spindle approach";
    }
}

}

#endif // PARAMETER_DESCRIPTIONS_H
