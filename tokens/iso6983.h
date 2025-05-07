#ifndef ISO6983_H
#define ISO6983_H

/*
 * Defines the common language as explained in ISO 6983-1:2009.
 *
 * This serves as the base onto which changes are grafted to create
 * the appropriate profile for a specific machine.
 */

#include <map>
#include <memory>
#include "tokens.h"

namespace NCParser {

class ISO6983 {
public:
    static std::shared_ptr<std::map<int,GCode>> gCodes() {return std::shared_ptr<std::map<int,GCode>>(new std::map<int,GCode>({
       {0, GCode{"Rapid positioning", {}, GCodes::rapid_positioning, Duration::FRC_A}},
       {1, GCode{"Linear interpolation", {}, GCodes::linear_interpolation, Duration::FRC_A}},
       {2, GCode{"Circular interpolation arc clockwise", {}, GCodes::circular_interpolation_cw, Duration::FRC_A}},
       {3, GCode{"Circular interpolation arc counter-clockwise", {}, GCodes::circular_interpolation_ccw, Duration::FRC_A}},
       {4, GCode{"Dwell", {}, GCodes::dwell, Duration::TBO}},
       {6, GCode{"Parabolic interpolation", {}, GCodes::parabolic_interpolation, Duration::FRC_A}},
       {9, GCode{"Exact stop", {}, GCodes::exact_stop, Duration::TBO}},
       {17, GCode{"XY plane selection", {}, GCodes::xy_plane_selection, Duration::FRC_B}},
       {18, GCode{"ZX plane selection", {}, GCodes::zx_plane_selection, Duration::FRC_B}},
       {19, GCode{"ZY plane selection", {}, GCodes::zy_plane_selection, Duration::FRC_B}},
       {33, GCode{"Thread cutting, constant lead", {}, GCodes::thread_cutting_constant_lead, Duration::FRC_A}},
       {34, GCode{"Thread cutting, increasing lead", {}, GCodes::thread_cutting_increasing_lead, Duration::FRC_A}},
       {35, GCode{"Thread cutting, decreasing lead", {}, GCodes::thread_cutting_decreasing_lead, Duration::FRC_A}},
       {40, GCode{"Cutter compensation cancel", {}, GCodes::cutter_compensation_cancel, Duration::FRC_D}},
       {41, GCode{"Cutter compensation left", {}, GCodes::cutter_compensation_left, Duration::FRC_D}},
       {42, GCode{"Cutter compensation right", {}, GCodes::cutter_compensation_right, Duration::FRC_D}},
       {43, GCode{"Tool offset positive", {}, GCodes::tool_offset_positive, Duration::FRC_D}},
       {44, GCode{"Tool offset negative", {}, GCodes::tool_offset_negative, Duration::FRC_D}},
       {53, GCode{"Dimension shift cancel", {}, GCodes::dimension_shift_cancel, Duration::FRC_F}},
       {54, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {55, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {56, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {57, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {58, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {59, GCode{"Zero shift", {}, GCodes::zero_shift, Duration::FRC_F}},
       {60, GCode{"Exact stop", {}, GCodes::exact_stop, Duration::FRC_G}},
       {63, GCode{"Tapping", {}, GCodes::tapping, Duration::TBO}},
       {64, GCode{"Continuous-path mode", {}, GCodes::continuous_path_mode, Duration::FRC_G}},
       {70, GCode{"Dimension input inch", {}, GCodes::dimension_input_inch, Duration::FRC_M}},
       {71, GCode{"Dimension input metric", {}, GCodes::dimension_input_metric, Duration::FRC_M}},
       {74, GCode{"Home position", {}, GCodes::home_position, Duration::TBO}},
       {80, GCode{"Fixed cycle cancel", {}, GCodes::fixed_cycle_cancel, Duration::FRC_E}},
       {81, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_81, Duration::FRC_E}},
       {82, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_82, Duration::FRC_E}},
       {83, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_83, Duration::FRC_E}},
       {84, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_84, Duration::FRC_E}},
       {85, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_85, Duration::FRC_E}},
       {86, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_86, Duration::FRC_E}},
       {87, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_87, Duration::FRC_E}},
       {88, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_88, Duration::FRC_E}},
       {89, GCode{"Fixed cycle", {}, GCodes::fixed_cycle_89, Duration::FRC_E}},
       {90, GCode{"Absolute dimension", {}, GCodes::absolute_dimension, Duration::FRC_I}},
       {91, GCode{"Incremental dimension", {}, GCodes::incremental_dimension, Duration::FRC_I}},
       {92, GCode{"Preload registers", {}, GCodes::preload_registers, Duration::TBO}},
       {93, GCode{"Inverse time", {}, GCodes::inverse_time, Duration::FRC_K}},
       {94, GCode{"Feed per minute", {}, GCodes::feed_per_minute, Duration::FRC_K}},
       {95, GCode{"Feed per revolution", {}, GCodes::feed_per_revolution, Duration::FRC_K}},
       {96, GCode{"Constant surface speed", {}, GCodes::constant_surface_speed, Duration::FRC_I}},
       {97, GCode{"Revolutions per minute", {}, GCodes::revolutions_per_minute, Duration::FRC_I}},
       }));};

    static std::shared_ptr<std::map<int,MCode>> mCodes() {return std::shared_ptr<std::map<int,MCode>>(new std::map<int,MCode>({
       {0, {"Program stop", MCodes::program_stop, Duration::TBO, ActionScope::AAM}},
       {1, {"Optional (planned) stop", MCodes::optional_stop, Duration::TBO, ActionScope::AAM}},
       {2, {"End of program", MCodes::end_of_program, Duration::TBO, ActionScope::AAM}},
       {3, {"Spindle CW", MCodes::spindle_cw, Duration::FRC, ActionScope::UndefinedScope}},
       {4, {"Spindle CCW", MCodes::spindle_ccw, Duration::FRC, ActionScope::UndefinedScope}},
       {5, {"Spindle OFF", MCodes::spindle_off, Duration::FRC, ActionScope::UndefinedScope}},
       {6, {"Tool change", MCodes::tool_change, Duration::TBO, ActionScope::UndefinedScope}},
       {10, {"Clamp workpiece", MCodes::clamp_workpiece, Duration::TBO, ActionScope::UndefinedScope}},
       {11, {"Unclamp workpiece", MCodes::unclamp_workpiece, Duration::TBO, ActionScope::UndefinedScope}},
       {30, {"End of data", MCodes::end_of_data, Duration::TBO, ActionScope::AAM}},
       {48, {"Cancel M49", MCodes::cancel_m49, Duration::None, ActionScope::UndefinedScope}},
       {49, {"Bypass override", MCodes::bypass_override, Duration::None, ActionScope::AAM_AWM}},
       {60, {"Workpiece change", MCodes::workpiece_change, Duration::TBO, ActionScope::UndefinedScope}},
       }));};

    static std::shared_ptr<std::map<int,FixedCycleDefinition>> fixedCycleDefinition() {
        return std::shared_ptr<std::map<int,FixedCycleDefinition>>(new std::map<int,FixedCycleDefinition>({
            {GCodes::fixed_cycle_81, {FixedCycleDefinition::FeedIn, FixedCycleDefinition::No,
                                               FixedCycleDefinition::None, FixedCycleDefinition::Rapid, true}},
            {GCodes::fixed_cycle_82, {FixedCycleDefinition::FeedIn, FixedCycleDefinition::Yes,
                                               FixedCycleDefinition::None, FixedCycleDefinition::Rapid, true}},
            {GCodes::fixed_cycle_83, {FixedCycleDefinition::Intermittent, FixedCycleDefinition::No,
                                               FixedCycleDefinition::None, FixedCycleDefinition::Rapid, true}},
            {GCodes::fixed_cycle_84, {FixedCycleDefinition::Forward_Spindle_Feed, FixedCycleDefinition::No,
                                               FixedCycleDefinition::Reverse, FixedCycleDefinition::FeedOut, true}},
            {GCodes::fixed_cycle_85, {FixedCycleDefinition::FeedIn, FixedCycleDefinition::No,
                                               FixedCycleDefinition::None, FixedCycleDefinition::FeedOut, true}},
            {GCodes::fixed_cycle_86, {FixedCycleDefinition::Start_Spindle_Feed, FixedCycleDefinition::No,
                                               FixedCycleDefinition::Stop, FixedCycleDefinition::Rapid, true}},
            {GCodes::fixed_cycle_87, {FixedCycleDefinition::Start_Spindle_Feed, FixedCycleDefinition::No,
                                               FixedCycleDefinition::Stop, FixedCycleDefinition::Manual, true}},
            {GCodes::fixed_cycle_88, {FixedCycleDefinition::Start_Spindle_Feed, FixedCycleDefinition::Yes,
                                               FixedCycleDefinition::Stop, FixedCycleDefinition::Manual, true}},
            {GCodes::fixed_cycle_89, {FixedCycleDefinition::FeedIn, FixedCycleDefinition::Yes,
                                               FixedCycleDefinition::None, FixedCycleDefinition::FeedOut, true}},
            }));
    };
};

};

#endif
