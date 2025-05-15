#ifndef PARSE_NODE_GEN_H
#define PARSE_NODE_GEN_H

#include <string>
#include <memory>
#include <variant>
#include <map>

namespace NCParser {

class parse_node;
typedef std::shared_ptr<parse_node> parse_node_p;
struct MachineParameters;

class parse_node_gen
{
public:
    struct machine_state {
        std::map<char,std::variant<int,double>> axis_positions = {
            {'X', 0},
            {'Y', 0},
            {'Z', 0}
        };
        enum feed_mode {
            FeedPerMinute,
            FeedPerRevolution
        } feed_mode;
        enum rpm_mode {
            ConstantSurfaceSpeed,
            RevolutionsPerMinute
        } rpm_mode;
    };

    parse_node_gen(MachineParameters *param): param(param) {}

    std::pair<std::string,machine_state> generate(parse_node_p block, machine_state machineState);
    void traverse(parse_node_p node);

protected:
    std::pair<std::string,machine_state> generate_internal();
    bool rerun = false;

    parse_node_p root;
    MachineParameters *param;
    machine_state state;

    std::string expr(parse_node_p n);
    bool g(int code);

    // General
    bool transform_coordinates(int code);
    bool feed_rpm(int code);

    // Cutting (generate_g_cutting.cpp)
    bool g_circular_interpolation_cw_func();
    bool g_circular_interpolation_ccw_func();
    bool g_linear_interpolation_func();
    bool g_linear_interpolation_with_collision_stop_func();
    bool g_linear_interpolation_with_feed_reduction_func();
    bool g_rapid_positioning_func();

    // Milling (generate_g_milling.cpp)

    // Settings (generate_g_settings.cpp)

    // Threads (generate_g_settings.cpp)

    // Tools (generate_g_settings.cpp)
};

}

#endif // PARSE_NODE_GEN_H
