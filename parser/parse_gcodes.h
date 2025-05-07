#ifndef PARSE_GCODES_H
#define PARSE_GCODES_H

#include "parse_node.h"

namespace NCParser {

class parser;

class parse_gcodes
{
public:
    parse_gcodes(parser *parent);

    parse_node *parse(int code);

protected:
    parse_node *linear_interpolation();
    parse_node *circular_interpolation();
    parse_node *dwell();
    parse_node *parabolic_interpolation();
    parse_node *exact_stop();
    parse_node *plane_selection(int first, int second);
    parse_node *thread_cutting_constant_lead();
    parse_node *thread_cutting_increasing_lead();
    parse_node *thread_cutting_decreasing_lead();
    parse_node *cutter_compensation_cancel();
    parse_node *cutter_compensation_left();
    parse_node *cutter_compensation_right();
    parse_node *tool_offset_positive();
    parse_node *tool_offset_negative();
    parse_node *dimension_shift_cancel();
    parse_node *zero_shift();
    parse_node *tapping();
    parse_node *continuous_path_mode();
    parse_node *dimension_input_inch();
    parse_node *dimension_input_metric();
    parse_node *home_position();
    parse_node *fixed_cycle_cancel();
    parse_node *fixed_cycle();
    parse_node *absolute_dimension();
    parse_node *incremental_dimension();
    parse_node *preload_registers();
    parse_node *inverse_time();
    parse_node *feed_per_minute();
    parse_node *feed_per_revolution();
    parse_node *constant_surface_speed();
    parse_node *revolutions_per_minute();

    parser *parent;
    int code;
};

};

#endif // PARSE_GCODES_H
