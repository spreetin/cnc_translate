#ifndef M_CODE_DESCRIPTIONS_H
#define M_CODE_DESCRIPTIONS_H

#include <string_view>
#include "m_codes.h"

namespace NCParser {

static constexpr std::string_view m_code_name(MCodesAvailable code){
    switch (code){
    case m_program_stop:
        return "Program stop";
    case m_optional_stop:
        return "Optional program stop";
    case m_end_of_program:
        return "End of program";
    case m_spindle_cw:
        return "Spindle rotation clockwise";
    case m_spindle_ccw:
        return "Spindle rotation counter-clockwise";
    case m_spindle_off:
        return "Spindle rotation off";
    case m_tool_change:
        return "Tool change";
    case m_clamp_workpiece:
        return "Clamp workpiece";
    case m_unclamp_workpiece:
        return "Unclamp workpiece";
    case m_end_of_data:
        return "End of program";
    case m_bypass_override_off:
        return "";
    case m_bypass_override:
        return "";
    case m_workpiece_change:
        return "Change workpiece";
    case m_activate_c_axis:
        return "Activate C axis";
    case m_deactivate_c_axis:
        return "Deactivate C axis";
    case m_activate_c_axis_opposing_spindle:
        return "Activate C axis on opposing spindle";
    case m_deactivate_c_axis_opposing_spindle:
        return "Deactivate C axis on opposing spindle";
    case m_c_axis_rotation_right:
        return "Rotate C axis right";
    case m_c_axis_rotation_left:
        return "Rotate C axis left";
    case m_double_dock_barrier_on:
        return "";
    case m_double_dock_barrier_off:
        return "";
    case m_thread_chamfer_on:
        return "";
    case m_thread_chamfer_off:
        return "";
    case m_chuck_barrier_on:
        return "";
    case m_chuck_barrier_off:
        return "";
    case m_thread_pitch_axis_x:
        return "";
    case m_thread_pitch_axis_z:
        return "";
    case m_thread_cut_straight_left:
        return "";
    case m_thread_cut_zigzag:
        return "";
    case m_thread_cut_straight_right:
        return "";
    case m_spindle_gear_neutral:
        return "";
    case m_spindle_gear_1:
        return "";
    case m_spindle_gear_2:
        return "";
    case m_spindle_gear_3:
        return "";
    case m_spindle_gear_4:
        return "";
    case m_nubdock_retract:
        return "";
    case m_nubdock:
        return "";
    case m_feed_rate_wait:
        return "Wait until designated feed speed is reached before operation";
    case m_feed_rate_nowait:
        return "Don't wait until designated feed speed is reached before operation";
    case m_spindle_rotation_nowait:
        return "";
    case m_thread_cycle_schema_1:
        return "";
    case m_thread_cycle_schema_2:
        return "";
    case m_thread_cycle_schema_3:
        return "";
    case m_scrub_cycle_noreturn:
        return "";
    case m_revolver_cw_locked:
        return "";
    case m_revolver_cw_unlocked:
        return "";
    case m_spindle_on_axis_c_on:
        return "";
    case m_spindle_on_axis_c_off:
        return "";
    case m_alarm_on_stm_time_passed_on:
        return "";
    case m_alarm_on_stm_time_passed_off:
        return "";
    case m_profile_for_fixed_cycle:
        return "";
    case m_thread_tool_rpm_nowait:
        return "";
    case m_c_axis_use_locking:
        return "";
    case m_c_axis_lock:
        return "Lock C axis";
    case m_c_axis_unlock:
        return "Unlock C axis";
    case m_nub_workmode_on:
        return "";
    case m_nub_workmode_off:
        return "";
    case m_feed_limit_manual_obey_on:
        return "";
    case m_feed_limit_manual_obey_off:
        return "";
    case m_rpm_limit_manual_obey_on:
        return "";
    case m_rpm_limit_manual_obey_off:
        return "";
    case m_single_block_obey_on:
        return "Honor single block mode";
    case m_single_block_obey_off:
        return "Don't honor single block mode";
    case m_nub_allowed_on_spindle_rotation_on:
        return "";
    case m_nub_allowed_on_spindle_rotation_off:
        return "";
    case m_chuck_opening_allowed_on_spindle_rotation_on:
        return "";
    case m_chuck_opening_allowed_on_spindle_rotation_off:
        return "";
    case m_wedge_track_cycle_single_dimension:
        return "";
    case m_wedge_track_cycle_single_zigzag:
        return "";
    case m_wedge_track_cycle_specific_cutting_amount:
        return "";
    case m_wedge_track_cycle_equal_cutting_amount:
        return "";
    case m_multiop_rpm_min:
        return "Minimum spindle rotation rate on multiop";
    case m_multiop_rpm_max:
        return "Maximum spindle rotation rate on multiop";
    case m_revolver_allowed_without_axis_c_on:
        return "Allow tool revolver rotation with unlocked C axis on";
    case m_revolver_allowed_without_axis_c_off:
        return "Allow tool revolver rotation with unlocked C axis off";
    case m_queueing:
        return "Wait for other subsystem(s)";
    case m_chip_transport_off:
        return "Chip transport off";
    case m_chip_transport_on:
        return "Chip transport on";
    case m_chuck_opening_allowed_on_spindle_rotation_off_b:
        return "";
    case m_chuck_opening_allowed_on_spindle_rotation_on_b:
        return "";
    case m_clamp_workpiece_b:
        return "Clamp workpiece in second subsystem";
    case m_compressed_air_1_off:
        return "Turn off first compressed air system";
    case m_compressed_air_1_on:
        return "Turn on first compressed air system";
    case m_compressed_air_2_off:
        return "Turn off second compressed air system";
    case m_compressed_air_2_on:
        return "Turn on second compressed air system";
    case m_compressed_air_3_off:
        return "Turn off third compressed air system";
    case m_compressed_air_3_on:
        return "Turn on third compressed air system";
    case m_compressed_air_4_off:
        return "Turn off fourth compressed air system";
    case m_compressed_air_4_on:
        return "Turn on fourth compressed air system";
    case m_coolant_1_off:
        return "";
    case m_coolant_1_on:
        return "";
    case m_coolant_1_high:
        return "";
    case m_coolant_2_off:
        return "";
    case m_coolant_2_on:
        return "";
    case m_coolant_2_high:
        return "";
    case m_coolant_3_off:
        return "";
    case m_coolant_3_on:
        return "";
    case m_coolant_3_high:
        return "";
    case m_coolant_4_off:
        return "";
    case m_coolant_4_on:
        return "";
    case m_coolant_4_high:
        return "";
    case m_data_transfer_request:
        return "";
    case m_revolver_ccw:
        return "Revolver direction counter-clockwise";
    case m_revolver_cw:
        return "Revolver direction clockwise";
    case m_revolver_stop:
        return "Stop revolver rotation";
    case m_spindle_rpm_variable_off:
        return "";
    case m_spindle_rpm_variable_on:
        return "";
    case m_unclamp_workpiece_b:
        return "Unclamp workpiece in second subsystem";
    case m_workpiece_change_profile:
        return "Workpiece change for profile material";
    }
    return "";
}

}

#endif // M_CODE_DESCRIPTIONS_H
