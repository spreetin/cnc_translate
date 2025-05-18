#ifndef PARSE_NODE_GEN_H
#define PARSE_NODE_GEN_H

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace NCParser {

class parse_node;
typedef std::shared_ptr<parse_node> parse_node_p;
struct MachineParameters;

/*!
 * \brief Takes a parse tree root node and generates code for the chosen machine from it
 */
class parse_node_gen
{
public:
    struct machine_state {
        std::map<char,double> axis_positions = {
            {'X', 0},
            {'Y', 0},
            {'Z', 0}
        };
        enum feed_mode {
            FeedPerMinute,
            FeedPerRevolution
        } feed_mode = FeedPerMinute;
        enum rpm_mode {
            ConstantSurfaceSpeed,
            RevolutionsPerMinute
        } rpm_mode = RevolutionsPerMinute;
        enum dimension_mode {
            AbsoluteCoordinates,
            IncrementalCoordinates,
        } dimension_mode = AbsoluteCoordinates;
        int subsystem = 1;
        std::map<std::string,double> variables;
    };

    parse_node_gen(MachineParameters *param, const parse_node_p &block, const machine_state &machineState)
        : param(param), root(block), state_base(machineState) {}

    inline std::string generate() {return generate_internal().first;}

protected:
    std::pair<std::string,machine_state> generate_internal();

    // Both generation and transformation
    void updateState(machine_state * state, int gcode = -1, int mcode = -1) const;

    // Generation methods
    std::pair<std::string,machine_state> generate_text() const;
    std::string expr(const parse_node_p &n) const;

    // Transformation methods
    void transformTree();
    std::vector<parse_node_p> squashTree(parse_node_p node);
    bool g(int code);
    bool m(int code);

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

    // Variables
    parse_node_p root;
    MachineParameters *param;
    machine_state state_base;
    machine_state transform_state;
};

}

#endif // PARSE_NODE_GEN_H
