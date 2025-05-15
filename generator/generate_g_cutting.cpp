#include "../tokens/g_codes.h"
#include <vector>
#include "parse_node_gen.h"
#include "../parser/parse_node.h"
#include "../tokens/tokens.h"
#include "../brands/parameters.h"
#include "../tokens/g_codes.h"

namespace NCParser {

bool parse_node_gen::g_circular_interpolation_cw_func(){
    return false;
}

bool parse_node_gen::g_circular_interpolation_ccw_func(){
    return false;
}

bool parse_node_gen::g_rapid_positioning_func(){
    return false;
}

bool parse_node_gen::g_linear_interpolation_func(){
    return false;
}

bool parse_node_gen::g_linear_interpolation_with_collision_stop_func()
{
    return false;
}

bool parse_node_gen::g_linear_interpolation_with_feed_reduction_func()
{
    return false;
}

}
