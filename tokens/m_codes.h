#ifndef M_CODES_H
#define M_CODES_H

#include <string>
#include <map>

namespace NCParser {

enum MCodesAvailable {
    program_stop,
    optional_stop,
    end_of_program,
    spindle_cw,
    spindle_ccw,
    spindle_off,
    tool_change,
    clamp_workpiece,
    unclamp_workpiece,
    end_of_data,
    cancel_m49,
    bypass_override,
    workpiece_change,
    activate_c_axis,
    deactivate_c_axis,
    activate_c_axis_opposing_spindle,
    deactivate_c_axis_opposing_spindle,
};

struct MCodeDescription {
    std::string name;
    std::string description;
};

std::map<int,std::string> mcode_descriptions = {
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

#endif // M_CODES_H
