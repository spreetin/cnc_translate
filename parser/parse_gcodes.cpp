#include "parse_gcodes.h"
#include "parser.h"
#include "../tokens/tokens.h"

namespace NCParser {

parse_gcodes::parse_gcodes(parser *parent) : parent(parent) {}

parse_node *parse_gcodes::parse(int code)
{
    this->code = code;
    switch (code){
    case GCodes::rapid_positioning:
    case GCodes::linear_interpolation:
        return linear_interpolation();
    case GCodes::circular_interpolation_cw:
    case GCodes::circular_interpolation_ccw:
        return circular_interpolation();
    case GCodes::dwell:
        return dwell();
    case GCodes::parabolic_interpolation:
        return parabolic_interpolation();
    case GCodes::exact_stop:
        return exact_stop();
    case GCodes::xy_plane_selection:
        return plane_selection(Token::x_word, Token::y_word);
    case GCodes::zx_plane_selection:
        return plane_selection(Token::z_word, Token::x_word);
    case GCodes::zy_plane_selection:
        return plane_selection(Token::z_word, Token::y_word);
    case GCodes::thread_cutting_constant_lead:
        return thread_cutting_constant_lead();
    case GCodes::thread_cutting_increasing_lead:
        return thread_cutting_increasing_lead();
    case GCodes::thread_cutting_decreasing_lead:
        return thread_cutting_decreasing_lead();
    case GCodes::cutter_compensation_cancel:
        return cutter_compensation_cancel();
    case GCodes::cutter_compensation_left:
        return cutter_compensation_left();
    case GCodes::cutter_compensation_right:
        return cutter_compensation_right();
    case GCodes::tool_offset_positive:
        return tool_offset_positive();
    case GCodes::tool_offset_negative:
        return tool_offset_negative();
    case GCodes::dimension_shift_cancel:
        return dimension_shift_cancel();
    case GCodes::zero_shift:
        return zero_shift();
    case GCodes::tapping:
        return tapping();
    case GCodes::continuous_path_mode:
        return continuous_path_mode();
    case GCodes::dimension_input_inch:
        return dimension_input_inch();
    case GCodes::dimension_input_metric:
        return dimension_input_metric();
    case GCodes::home_position:
        return home_position();
    case GCodes::fixed_cycle_cancel:
        return fixed_cycle_cancel();
    case GCodes::fixed_cycle_81:
    case GCodes::fixed_cycle_82:
    case GCodes::fixed_cycle_83:
    case GCodes::fixed_cycle_84:
    case GCodes::fixed_cycle_85:
    case GCodes::fixed_cycle_86:
    case GCodes::fixed_cycle_87:
    case GCodes::fixed_cycle_88:
    case GCodes::fixed_cycle_89:
        return fixed_cycle();
    case GCodes::absolute_dimension:
        return absolute_dimension();
    case GCodes::incremental_dimension:
        return incremental_dimension();
    case GCodes::preload_registers:
        return preload_registers();
    case GCodes::inverse_time:
        return inverse_time();
    case GCodes::feed_per_minute:
        return feed_per_minute();
    case GCodes::feed_per_revolution:
        return feed_per_revolution();
    case GCodes::constant_surface_speed:
        return constant_surface_speed();
    case GCodes::revolutions_per_minute:
        return revolutions_per_minute();
    default:
        return new parse_node(Token::unknown_code);
    }
}

parse_node *parse_gcodes::linear_interpolation()
{
    std::vector<parse_node*> children = parent->optionlist();
    return new parse_node(Token::g_word, code, children);
}

parse_node *parse_gcodes::circular_interpolation()
{
    std::vector<parse_node*> children = parent->optionlist();
    return new parse_node(Token::g_word, code, children);
}

parse_node *parse_gcodes::dwell()
{

}

parse_node *parse_gcodes::parabolic_interpolation()
{

}

parse_node *parse_gcodes::exact_stop()
{

}

parse_node *parse_gcodes::plane_selection(int first, int second)
{

}

parse_node *parse_gcodes::thread_cutting_constant_lead()
{

}

parse_node *parse_gcodes::thread_cutting_increasing_lead()
{

}

parse_node *parse_gcodes::thread_cutting_decreasing_lead()
{

}

parse_node *parse_gcodes::cutter_compensation_cancel()
{

}

parse_node *parse_gcodes::cutter_compensation_left()
{

}

parse_node *parse_gcodes::cutter_compensation_right()
{

}

parse_node *parse_gcodes::tool_offset_positive()
{

}

parse_node *parse_gcodes::tool_offset_negative()
{

}

parse_node *parse_gcodes::dimension_shift_cancel()
{

}

parse_node *parse_gcodes::zero_shift()
{

}

parse_node *parse_gcodes::tapping()
{

}

parse_node *parse_gcodes::continuous_path_mode()
{

}

parse_node *parse_gcodes::dimension_input_inch()
{

}

parse_node *parse_gcodes::dimension_input_metric()
{

}

parse_node *parse_gcodes::home_position()
{

}

parse_node *parse_gcodes::fixed_cycle_cancel()
{

}

parse_node *parse_gcodes::fixed_cycle()
{

}

parse_node *parse_gcodes::absolute_dimension()
{

}

parse_node *parse_gcodes::incremental_dimension()
{

}

parse_node *parse_gcodes::preload_registers()
{

}

parse_node *parse_gcodes::inverse_time()
{

}

parse_node *parse_gcodes::feed_per_minute()
{

}

parse_node *parse_gcodes::feed_per_revolution()
{

}

parse_node *parse_gcodes::constant_surface_speed()
{

}

parse_node *parse_gcodes::revolutions_per_minute()
{

}

};
