#ifndef G_CODE_DESCRIPTION_H
#define G_CODE_DESCRIPTION_H

#include <string>
#include "g_codes.h"

namespace NCParser {

const std::string g_code_name(GCodesAvailable code){
    switch (code){
    case auto_geometry_function:
        return "";
    case g_absolute_dimension:
        return "Absolute dimension";
    case g_bevel_cycle_against_spindle:
        return "Bevel cycle from spindle direction";
    case g_bevel_cycle_towards_spindle:
        return "Bevel cycle towards spindle";
    case g_chuck_transfer_under_torque:
        return "Transfer between chucks under torque";
    case g_circular_interpolation_ccw:
        return "Circular interpolation arc counter-clockwise";
    case g_circular_interpolation_cw:
        return "Circular interpolation arc clockwise";
    case g_circular_interpolation_ccw_milling_on_turning:
        return "";
    case g_circular_interpolation_cw_milling_on_turning:
        return "";
    case g_constant_surface_speed:
        return "Constant surface speed";
    case g_continuous_path_mode:
        return "Continuous-path mode";
    case g_coordinate_system_set:
        return "";
    case g_cutter_compensation_auto:
        return "Cutter compensation automatic";
    case g_cutter_compensation_cancel:
        return "Cutter compensation cancel";
    case g_cutter_compensation_left:
        return "Cutter compensation left";
    case g_cutter_compensation_right:
        return "Cutter compensation right";
    case g_cutting_cycle_concave_tool_x:
        return "";
    case g_cutting_cycle_convex_tool_x:
        return "";
    case g_cutting_cycle_same_contour:
        return "";
    case g_cutting_cycle_straight_with_fallback:
        return "";
    case g_cutting_cycle_straight_with_fallback_z:
        return "";
    case g_cutting_cycle_z:
        return "";
    case g_cycle_repetition:
        return "";
    case g_dimension_input_inch:
        return "Dimension input inch";
    case g_dimension_input_metric:
        return "Dimension input metric";
    case g_dimension_shift_cancel:
        return "Dimension shift cancel";
    case g_drill_cycle_step_distance:
        return "Step diameter for drill cycle";
    case g_drill_cycle_step_number:
        return "Step number for drill cycle";
    case g_drill_cycle_z_axis:
        return "";
    case g_dwell:
        return "Dwell (pause)";
    case g_exact_stop:
        return "";
    case g_exact_stop_block:
        return "";
    case g_feed_correction:
        return "Feed correction ON";
    case g_feed_correction_off:
        return "Feed correction OFF";
    case g_feed_per_minute:
        return "Feed per minute";
    case g_feed_per_revolution:
        return "Feed per revolution";
    case g_fixed_cycle:
        return "Fixed cycle";
    case g_fixed_cycle_cancel:
        return "Fixed cycle cancel";
    case g_home_position:
        return "Home position";
    case g_home_position_ordered_xz:
        return "Home position on first X then Z axis'";
    case g_home_position_ordered_zx:
        return "Home position on first Z then X axis'";
    case g_home_position_x:
        return "Home position on the X axis";
    case g_home_position_y:
        return "Home position on the Y axis";
    case g_home_position_z:
        return "Home position on the Z axis";
    case g_incremental_dimension:
        return "Incremental dimension";
    case g_initial_level_return_fixed:
        return "";
    case g_inverse_time:
        return "Inverse time";
    case g_linear_interpolation:
        return "Linear interpolation";
    case g_linear_interpolation_with_collision_stop:
        return "Linear interpolation, with halt on collision";
    case g_linear_interpolation_with_feed_reduction:
        return "Linear interpolation, with reduced feed";
    case g_load_subprogram:
        return "Load sub program";
    case g_milling_interpolation_off:
        return "Milling interpolation OFF";
    case g_milling_interpolation_on:
        return "Milling interpolation ON";
    case g_milling_line_end_face:
        return "Line milling on end face";
    case g_milling_curve_cw_end_face:
        return "Curved milling clockwise on end face";
    case g_milling_curve_ccw_end_face:
        return "Curved milling counter-clockwise on end face";
    case g_parabolic_interpolation:
        return "Parabolic interpolation";
    case g_preload_registers:
        return "Preload registers";
    case g_r_point_return_fixed:
        return "";
    case g_rapid_direct_to_reference:
        return "Rapid home to reference point";
    case g_rapid_positioning:
        return "Rapid positioning";
    case g_rapid_relative_to_reference:
        return "Rapid positioning relative to reference point";
    case g_revolutions_per_minute:
        return "Revolutions per minute";
    case g_rpm_limit:
        return "Revolutions per minute limit";
    case g_servo_error_compensation_off:
        return "Compensation for servo error OFF";
    case g_servo_error_compensation_on:
        return "Compensation for servo error ON";
    case g_tapping:
        return "";
    case g_thread_cutting_circular_ccw:
        return "Thread cutting, circular, counter-clockwise";
    case g_thread_cutting_circular_cw:
        return "Thread cutting, circular, clockwise";
    case g_thread_cutting_constant_lead:
        return "Thread cutting, constant lead";
    case g_thread_cutting_cycle:
        return "Thread cutting cycle";
    case g_thread_cutting_cycle_end_face:
        return "Thread cutting cycle on end face";
    case g_thread_cutting_decreasing_lead:
        return "Thread cutting, decreasing lead";
    case g_thread_cutting_end_face:
        return "Thread cutting on end face";
    case g_thread_cutting_increasing_lead:
        return "Thread cutting, increasing lead";
    case g_thread_cutting_multistep:
        return "Thread cutting with changing conicity";
    case g_thread_cutting_variable_lead:
        return "Thread cutting, variable lead";
    case g_tool_offset_negative:
        return "Tool offset negative";
    case g_tool_offset_positive:
        return "Tool offset positive";
    case g_torque_limit_chuck_transfer:
        return "Limit torque on transfer between chucks";
    case g_torque_limit_chuck_transfer_off:
        return "Cancel limit torque on transfer between chucks";
    case g_transfer_chamfer:
        return "Chamfer on transfer between interpolations";
    case g_transfer_radius:
        return "Radius on transfer between interpolations";
    case g_wedge_cut_cycle_lengthwise:
        return "Milled wedge cut cycle, lengthwise";
    case g_xy_plane_selection:
        return "XY plane selection";
    case g_xz_cylindrical_plane_selection:
        return "XZ cylindrical plane selection";
    case g_yz_cylindrical_plane_selection:
        return "YZ cylindrical plane selection";
    case g_yz_plane_selection:
        return "YZ plane selection";
    case g_zero_set:
        return "Set zero point";
    case g_zero_shift:
        return "Zero shift";
    case g_zx_plane_selection:
        return "ZX plane selection";
    case g_zy_plane_selection:
        return "ZY plane selection";
    }
    return "";
}

}

#endif // G_CODE_DESCRIPTION_H
