#include <cmath>
#include <iostream>
#include <sstream>
#include "parse_node_gen.h"
#include "../brands/parameters.h"
#include "../parser/parse_node.h"
#include "../tokens/tokens.h"
#include "../errors/errors.h"

namespace NCParser {

/*!
 * \brief Transforms the parse tree for the machine and then generates code text
 * \return The text representation and the machine state by the end of parsing
 */
std::pair<std::string, parse_node_gen::machine_state> parse_node_gen::generate_internal()
{
    // Do all needed tree transformations
    transformTree();

    return generate_text();
}

/*!
 * \brief Takes a valid, already transformed, parse tree and generates code text
 * \return The text representation and the machine state by the end of parsing
 */
std::pair<std::string, parse_node_gen::machine_state> parse_node_gen::generate_text() const
{
    auto state = state_base;

    // If this node is only an expression, return it
    if (!root->childCount() || (root->type() != Token::block && root->type() != Token::list)){
        return {expr(root), state};
    }
    std::stringstream ss;

    // Print the contents of block
    int g_code = -1;
    for (auto &n : root->children()){
        switch (n->type()){
        case Token::queueing:
            if (param->queueing.has_queueing && param->queueing.queueing_single_char){
                ss << param->queueing.queueing_single_char;
            } else if(param->queueing.has_queueing && param->queueing.queueing_string.size()){
                ss << param->queueing.queueing_string;
            } else {
                std::cerr << (error{error::Generating, -1, -1, "No queueing enabled. Ignoring."}).to_string();
            }
            break;
        case Token::queueing_num:
            if (param->queueing.has_queueing && param->queueing.queueing_numbered_single_char){
                ss << param->queueing.queueing_numbered_single_char << n->stringValue();
            } else if(param->queueing.has_queueing && param->queueing.queueing_numbered_string.size()){
                ss << param->queueing.queueing_numbered_string << n->stringValue();
            } else {
                std::cerr << (error{error::Generating, -1, -1, "No queueing enabled. Ignoring."}).to_string();
            }
            break;
        case Token::g_word:
        {
            g_code = n->intValue();
            if (param->g.contains(g_code)){
                auto g_def = param->g[g_code];
                int vIndex = g_def.word.index();
                ss << "G";
                switch (vIndex){
                case 0:
                    ss << std::to_string(std::get<int>(g_def.word)) << " "; break;
                case 1:
                    // TODO Implement handling
                    ss << "N/A ";
                    break;
                case 2:
                    ss << std::get<std::string>(g_def.word) << " "; break;
                }
            } else {
                std::cerr << (error{error::Generating, -1, -1, "G-code not handled: " + std::to_string(g_code)}).to_string();
            }

        } break;
        case Token::m_word:
            if (param->m.contains(n->intValue())){
                ss << "M" << std::to_string(param->m[n->intValue()].num) << " ";
            } else {
                // TODO Try to handle non-existant G-codes
                std::cerr << (error{error::Generating, -1, -1, "No translation found for M-code with ID: " + n->stringValue()}).to_string();
            }
            break;
        case Token::subsystem_select:
        {
            if (param->subsystem.symbol == Token::g_word){
                switch (n->intValue()){
                case 1:
                    if (param->g.contains(g_use_subsystem_a)){
                        ss << "G" << std::to_string(std::get<int>(param->g[g_use_subsystem_a].word));
                    }
                    break;
                case 2:
                    if (param->g.contains(g_use_subsystem_b)){
                        ss << "G" << std::to_string(std::get<int>(param->g[g_use_subsystem_b].word));
                    }
                    break;
                case 3:
                    if (param->g.contains(g_use_subsystem_c)){
                        ss << "G" << std::to_string(std::get<int>(param->g[g_use_subsystem_c].word));
                    }
                    break;
                case 4:
                    if (param->g.contains(g_use_subsystem_d)){
                        ss << "G" << std::to_string(std::get<int>(param->g[g_use_subsystem_d].word));
                    }
                    break;
                }
            } else {
                ss << param->subsystem.symbol << std::to_string(n->intValue());
            }
        } break;
        case Token::n_word:
            ss << "N" << n->stringValue() << " ";
            break;
        case Token::left_over_data:
            std::cerr << (error{}).to_string();
            break;
        case Token::block_function:
            // TODO Implement
            break;
        case Token::parameter:
        {
            auto g_def = param->g[g_code];
            int id = n->intValue();
            char m = '\0';
            for (auto i : g_def.parameter_types){
                if (i.second == id){
                    m = i.first;
                    break;
                }
            }
            if (!m){
                for (auto i : param->parameter.defaults){
                    if (i.second == id){
                        m = i.first;
                        break;
                    }
                }
            }
            if (!m){
                std::cerr << (error{error::Generating, -1, -1, "Can't match parameter with value " + std::to_string(id)
                                                                   + " on G with code " + std::to_string(g_code)}).to_string();
                break;
            }
            ss << m;
            ss << expr(n->getChild(0)) << " ";
        } break;
        case Token::prg_name:
            for (auto i : param->parameter.defaults){
                if (i.second == param_prg_name){
                    if (n->childCount()){
                        ss << i.first << expr(n->getChild(0));
                    } else {
                        ss << i.first << n->stringValue();
                    }

                    break;
                }
            }
            break;
        case Token::comment:
            if (param->comments.use_parenthesis){
                ss << "(" << n->stringValue() << ")";
            } else if (param->comments.use_semicolon){
                ss << ";" << n->stringValue();
            } else {
                std::cerr << (error{error::Generating, -1, -1, "No comment format enabled, comment ignored."}).to_string();
            }
            break;
        case Token::block:
        case Token::list:
        // Run recursively if root is not a block, or contains sublists of blocks
        {
            auto r = parse_node_gen(param, n, state).generate_internal();
            state = r.second;
            ss << r.first; break;
        }
        break;
        default:
            ss << expr(n);
        }
    }
    ss << std::endl;
    return {ss.str(), state};
}

/*!
 * @brief Does transformations on the tree adapting it to the chosen output machine.
 *
 * Processes the tree, starting from the current root node for the object and transforms it
 * as needed for the chosen output machine. The original parsing tree is not touched, instead
 * new tree nodes are created as need when the parse tree needs to be edited.
 *
 * Existing nodes or data members are reused when possible though, so deleting any part of
 * the original parse tree while an instance of this class exists referencing the same tree
 * can cause errors, crashes or faulty output.
 */
void parse_node_gen::transformTree()
{
    bool rerun = false; // Expect that tree is useable

    // Check for, and complete, transformations based on G- or M-code
    // Keep repeating this until no node contains a code causing a transformation
    do {
        for (auto &n : root->children()){
            if (n->type() == Token::g_word){
                if ((rerun = g(n->intValue())))
                    break;
            } else if (n->type() == Token::m_word){
                if ((rerun = m(n->intValue())))
                    break;
            }
        }
    } while (rerun);

    // Squash unneccessary added tree depth
    root = parse_node_p(new parse_node(Token::list, squashTree(root)));

    // Check for unneccesary setting and resetting of the same function, like setting
    // G90 and then immideately setting G91. Also remove empty blocks;
    transform_state = state_base;
    std::vector<parse_node_p> newRoot;
    for (auto &n : root->children()){
        auto childList = n->children();
        std::erase_if(childList, [this]<typename T>(const T &p){
            if (p->type() == Token::g_word && p->intValue() == g_absolute_dimension
                && transform_state.dimension_mode == machine_state::AbsoluteCoordinates){
                return true;
            } else if (p->type() == Token::g_word && p->intValue() == g_incremental_dimension
                       && transform_state.dimension_mode == machine_state::IncrementalCoordinates){
                return true;
            }
            return false;
        });
    }
    auto childList = root->children();
    std::erase_if(childList, [this]<typename T>(const T &p){
        if (!p->childCount())
            return true;
        return false;
    });
}

/*!
 * \brief Recursively remove nested block lists, creating a flat list of blocks in the root node
 * \param node Root node to flatten from
 * \return A list of nodes extracted from the subtree
 */
std::vector<parse_node_p> parse_node_gen::squashTree(parse_node_p node)
{
    std::vector<parse_node_p> result;
    for (auto &n : node->children()){
        if (n->type() == Token::list){
            for (auto &n1 : squashTree(n)){
                result.push_back(n1);
            }
        } else {
            result.push_back(n);
        }
    }
    return result;
}

/*!
 * \brief Generate text representation of value expressions
 *
 * Expressions are such parts of code that doesn't have any side effects, instead just
 * becomes a (usually) numeric value when processed by the machine. This includes
 * numeric literals, variable references, numeric operators and built-in functions
 * that operate on numeric values, such as round, square root and equality checking.
 *
 * \param n Root node of the expression
 * \return A string representation of the expression
 */
std::string parse_node_gen::expr(const parse_node_p &n) const
{
    std::stringstream ss;
    static const auto get_func = [](const int v, const std::map<std::string,int> * m) -> std::string{
        for (auto &i : *m){
            if (i.second == v){
                return i.first;
            }
        }
        // TODO Simulate missing, if needed for some machines
        std::cerr << (error{error::Generating, -1, -1, "Function not translateable"}).to_string();
        return "";
    };
    const auto print_square = [&]<typename T>(const parse_node_p n, const T d){
        ss << "[" << expr(n->getChild(0)) << " " << d << " " << expr(n->getChild(1)) << "]";
    };

    switch (n->type()){
    // Binary
    case Token::equals:
        if (n->childCount() > 1){
            ss << expr(n->getChild(0)) << " = " << expr(n->getChild(1));
        } else if (n->childCount() == 1){
            ss << " = " << expr(n->getChild(0));
        }
        break;
    case Token::star:
    case Token::plus:
    case Token::minus:
    case Token::slash:
        print_square(n, n->type()); break;
    case Token::binary_functions:
        print_square(n, get_func(n->intValue(), &(param->functions.binary))); break;
    // Values
    case Token::num_literal:
    case Token::string:
        ss << n->stringValue(); break;
    case Token::num_int:
        ss << std::to_string(n->intValue()); break;
    case Token::num_float:
        // TODO Implement better float handling, to fit different machine parsing requirements
        ss << std::to_string(n->doubleValue()); break;
    case Token::variable:
    {
        if (param->variables.variable_marker.size()){
            for (char c : param->variables.variable_marker){
                ss << c << n->stringValue();
                break;
            }
        } else {
            char def = indexIn(param->parameter.defaults, param_variable);
            if (def >= 0){
                ss << param->parameter.defaults[def] << n->stringValue();
            } else {
                // TODO Handle regex defined variables
            }
        }
        break;
    }
    // Unary
    case Token::unary_function:
        ss << get_func(n->intValue(), &(param->functions.unary)) << "[" << expr(n->getChild(0)) << "]"; break;
    // Unclear
    case Token::multary_functions:
        break;
    case Token::period:
    case Token::percent:
        ss << n->type(); break;
    }
    return ss.str();
}

/*!
 * \brief Tests G-codes for needed transformations, and applies them
 *
 * This function will first check for some common issues (like incremental dimenstions
 * instead of incremental mode when the machine doesn't support those). If those are
 * found it will fix for them and return, so we can take another pass.
 *
 * If no such general issue is founc, a dedicated translation function for each defined
 * G-code is called, if such a function has been created.
 *
 * The default behaviour if the function can't find any defined transformations is to
 * return false (no transformation done), even if the code provided wouldn't parse
 * on the target machine. It isn't a compatibility checker, just a best attempt to
 * mangle code into compatibility.
 *
 * \param code G code type to do transformation tests for
 * \return true if a transformation has been done, otherwise false
 */
bool parse_node_gen::g(int code)
{
    if (transform_coordinates(code))
        return true;
    if (feed_rpm(code))
        return true;
    switch ((GCodesAvailable)code){
    case g_absolute_dimension: break;
    case g_auto_geometry_function: break;
    case g_bevel_cycle_against_spindle: break;
    case g_bevel_cycle_towards_spindle: break;
    case g_c_axis_mode: break;
    case g_chuck_transfer_under_torque: break;
    case g_circular_interpolation_ccw: return g_circular_interpolation_ccw_func();
    case g_circular_interpolation_ccw_milling_on_turning: break;
    case g_circular_interpolation_cw: return g_circular_interpolation_cw_func();
    case g_circular_interpolation_cw_milling_on_turning: break;
    case g_constant_surface_speed: break;
    case g_continuous_path_mode: break;
    case g_coordinate_cartesian_to_polar: break;
    case g_coordinate_cartesian_to_polar_off: break;
    case g_coordinate_system_set: break;
    case g_cutter_compensation_auto: break;
    case g_cutter_compensation_cancel: break;
    case g_cutter_compensation_left: break;
    case g_cutter_compensation_right: break;
    case g_cutting_cycle_concave_tool_x: break;
    case g_cutting_cycle_convex_tool_x: break;
    case g_cutting_cycle_same_contour: break;
    case g_cutting_cycle_straight_with_fallback: break;
    case g_cutting_cycle_straight_with_fallback_z: break;
    case g_cutting_cycle_z: break;
    case g_cycle_repetition: break;
    case g_dimension_input_inch: break;
    case g_dimension_input_metric: break;
    case g_dimension_shift_cancel: break;
    case g_drill_cycle_step_distance: break;
    case g_drill_cycle_step_number: break;
    case g_drill_cycle_z_axis: break;
    case g_dwell: break;
    case g_exact_stop: break;
    case g_exact_stop_block: break;
    case g_feed_correction: break;
    case g_feed_correction_off: break;
    case g_feed_per_minute: break;
    case g_feed_per_revolution: break;
    case g_fixed_cycle: break;
    case g_fixed_cycle_cancel: break;
    case g_home_position: break;
    case g_home_position_ordered_xz: break;
    case g_home_position_ordered_zx: break;
    case g_home_position_x: break;
    case g_home_position_y: break;
    case g_home_position_z: break;
    case g_incremental_dimension: break;
    case g_initial_level_return_fixed: break;
    case g_inverse_time: break;
    case g_linear_interpolation: return g_linear_interpolation_func();
    case g_linear_interpolation_with_collision_stop: return g_linear_interpolation_with_collision_stop_func();
    case g_linear_interpolation_with_feed_reduction: return g_linear_interpolation_with_feed_reduction_func();
    case g_load_subprogram: break;
    case g_milling_curve_ccw_end_face: break;
    case g_milling_curve_cw_end_face: break;
    case g_milling_interpolation_off: break;
    case g_milling_interpolation_on: break;
    case g_milling_line_end_face: break;
    case g_milling_tool_compensation_cancel: break;
    case g_milling_tool_compensation_left: break;
    case g_milling_tool_compensation_right: break;
    case g_parabolic_interpolation: break;
    case g_preload_registers: break;
    case g_r_point_return_fixed: break;
    case g_rapid_direct_to_reference: break;
    case g_rapid_positioning: return g_rapid_positioning_func();
    case g_rapid_relative_to_reference: break;
    case g_revolutions_per_minute: break;
    case g_rpm_limit: break;
    case g_servo_error_compensation_off: break;
    case g_servo_error_compensation_on: break;
    case g_tapping: break;
    case g_thread_cutting_circular_ccw: break;
    case g_thread_cutting_circular_cw: break;
    case g_thread_cutting_constant_lead: break;
    case g_thread_cutting_cycle: break;
    case g_thread_cutting_cycle_end_face: break;
    case g_thread_cutting_decreasing_lead: break;
    case g_thread_cutting_end_face: break;
    case g_thread_cutting_increasing_lead: break;
    case g_thread_cutting_multistep: break;
    case g_thread_cutting_variable_lead: break;
    case g_tool_offset_negative: break;
    case g_tool_offset_positive: break;
    case g_torque_limit_chuck_transfer: break;
    case g_torque_limit_chuck_transfer_off: break;
    case g_transfer_chamfer: break;
    case g_transfer_radius: break;
    case g_turning_mode: break;
    case g_use_subsystem_a: break;
    case g_use_subsystem_b: break;
    case g_use_subsystem_c: break;
    case g_use_subsystem_d: break;
    case g_wedge_cut_cycle_lengthwise: break;
    case g_xy_plane_selection: break;
    case g_xz_cylindrical_plane_selection: break;
    case g_y_axis_mode: break;
    case g_yz_cylindrical_plane_selection: break;
    case g_yz_plane_selection: break;
    case g_zero_set: break;
    case g_zero_shift: break;
    case g_zx_plane_selection: break;
    case g_zy_plane_selection: break;
    }
    return false;
}

/*!
 * \brief Tests M-codes for needed transformations, and applies them
 *
 * This function will simply call a dedicated translation function for each defined
 * M-code, if such a function has been created, or in applicable cases a general
 * transformation function for codes that can be handled in a general way.
 *
 * The default behaviour if the function can't find any defined transformations is to
 * return false (no transformation done), even if the code provided wouldn't parse
 * on the target machine. It isn't a compatibility checker, just a best attempt to
 * mangle code into compatibility.
 *
 * \param code M code type to do transformation tests for
 * \return true if a transformation has been done, otherwise false
 */
bool parse_node_gen::m(int code)
{
    switch ((MCodesAvailable)code){
    case m_activate_c_axis: break;
    case m_activate_c_axis_opposing_spindle: break;
    case m_alarm_on_stm_time_passed_off: break;
    case m_alarm_on_stm_time_passed_on: break;
    case m_bypass_override: break;
    case m_bypass_override_off: break;
    case m_c_axis_lock: break;
    case m_c_axis_rotation_left: break;
    case m_c_axis_rotation_right: break;
    case m_c_axis_unlock: break;
    case m_c_axis_use_locking: break;
    case m_chip_transport_off: break;
    case m_chip_transport_on: break;
    case m_chuck_barrier_off: break;
    case m_chuck_barrier_on: break;
    case m_chuck_opening_allowed_on_spindle_rotation_off: break;
    case m_chuck_opening_allowed_on_spindle_rotation_on: break;
    case m_chuck_opening_allowed_on_spindle_rotation_off_b: break;
    case m_chuck_opening_allowed_on_spindle_rotation_on_b: break;
    case m_clamp_workpiece: break;
    case m_clamp_workpiece_b: break;
    case m_compressed_air_1_off: break;
    case m_compressed_air_1_on: break;
    case m_compressed_air_2_off: break;
    case m_compressed_air_2_on: break;
    case m_compressed_air_3_off: break;
    case m_compressed_air_3_on: break;
    case m_compressed_air_4_off: break;
    case m_compressed_air_4_on: break;
    case m_coolant_1_off: break;
    case m_coolant_1_on: break;
    case m_coolant_1_high: break;
    case m_coolant_2_off: break;
    case m_coolant_2_on: break;
    case m_coolant_2_high: break;
    case m_coolant_3_off: break;
    case m_coolant_3_on: break;
    case m_coolant_3_high: break;
    case m_coolant_4_off: break;
    case m_coolant_4_on: break;
    case m_coolant_4_high: break;
    case m_data_transfer_request: break;
    case m_deactivate_c_axis: break;
    case m_deactivate_c_axis_opposing_spindle: break;
    case m_double_dock_barrier_off: break;
    case m_double_dock_barrier_on: break;
    case m_end_of_data: break;
    case m_end_of_program: break;
    case m_feed_limit_manual_obey_off: break;
    case m_feed_limit_manual_obey_on: break;
    case m_feed_rate_nowait: break;
    case m_feed_rate_wait: break;
    case m_multiop_rpm_max: break;
    case m_multiop_rpm_min: break;
    case m_nub_allowed_on_spindle_rotation_off: break;
    case m_nub_allowed_on_spindle_rotation_on: break;
    case m_nub_workmode_off: break;
    case m_nub_workmode_on: break;
    case m_nubdock: break;
    case m_nubdock_retract: break;
    case m_optional_stop: break;
    case m_profile_for_fixed_cycle: break;
    case m_program_stop: break;
    case m_queueing: break;
    case m_revolver_allowed_without_axis_c_off: break;
    case m_revolver_allowed_without_axis_c_on: break;
    case m_revolver_ccw: break;
    case m_revolver_cw: break;
    case m_revolver_cw_locked: break;
    case m_revolver_cw_unlocked: break;
    case m_revolver_stop: break;
    case m_rpm_limit_manual_obey_off: break;
    case m_rpm_limit_manual_obey_on: break;
    case m_scrub_cycle_noreturn: break;
    case m_single_block_obey_off: break;
    case m_single_block_obey_on: break;
    case m_spindle_ccw: break;
    case m_spindle_cw: break;
    case m_spindle_gear_1: break;
    case m_spindle_gear_2: break;
    case m_spindle_gear_3: break;
    case m_spindle_gear_4: break;
    case m_spindle_gear_neutral: break;
    case m_spindle_off: break;
    case m_spindle_on_axis_c_off: break;
    case m_spindle_on_axis_c_on: break;
    case m_spindle_rotation_nowait: break;
    case m_spindle_rpm_variable_off: break;
    case m_spindle_rpm_variable_on: break;
    case m_thread_chamfer_off: break;
    case m_thread_chamfer_on: break;
    case m_thread_cut_straight_left: break;
    case m_thread_cut_straight_right: break;
    case m_thread_cut_zigzag: break;
    case m_thread_cycle_schema_1: break;
    case m_thread_cycle_schema_2: break;
    case m_thread_cycle_schema_3: break;
    case m_thread_pitch_axis_x: break;
    case m_thread_pitch_axis_z: break;
    case m_thread_tool_rpm_nowait: break;
    case m_tool_change: break;
    case m_unclamp_workpiece: break;
    case m_unclamp_workpiece_b: break;
    case m_wedge_track_cycle_equal_cutting_amount: break;
    case m_wedge_track_cycle_single_dimension: break;
    case m_wedge_track_cycle_single_zigzag: break;
    case m_wedge_track_cycle_specific_cutting_amount: break;
    case m_workpiece_change: break;
    case m_workpiece_change_profile: break;
    }

    return false;
}

/*!
 * \brief Checks for unsupported coordinate specification modes and tries to fix them
 *
 * Specifically what this function looks for are incremental coordinates specified by
 * parameter words while the machine is in absolute dimension mode, and the machine
 * doesn't support incremental word parameters.
 *
 * Also circle definitions using either circle center coordinates or a radius, when
 * the machine doesn't support this mode of specifying curves. They will then be
 * converted to the other type of parameter.
 *
 * \param code G code type currently being checked
 * \return true if a transformation has been applied, false otherwise
 */
bool parse_node_gen::transform_coordinates(int code)
{
    // TODO Handle X/Y/Z coordinates while in incremental mode if machine has U/W/V
    auto nodes = root->children();
    bool incremental_to_absolute = false;
    bool radius_to_coord = false;
    bool coord_to_radius = false;
    for (auto &n : nodes){
        if (n->type() == Token::parameter){
            int param_type = n->intValue();
            bool found = false;
            for (auto &m : {param->g[code].parameter_types, param->parameter.defaults}){
                for (auto &p : m){
                    if (p.second == param_type){
                        found = true;
                        break;
                    }
                }
            }
            if (!found){
                switch (n->intValue()){
                case param_incremental_x:
                case param_incremental_y:
                case param_incremental_z:
                case param_circle_center_incr_x:
                case param_circle_center_incr_z:
                    incremental_to_absolute = true; break;
                case param_circle_center_x:
                case param_circle_center_z:
                    coord_to_radius = true; break;
                case param_circle_radius:
                    radius_to_coord = true; break;
                default: break;
                }
            }
        }
    }
    if (!incremental_to_absolute && !radius_to_coord && !coord_to_radius)
        return false;
    parse_node_p newRoot = parse_node_p{new parse_node(Token::list)};
    parse_node_p newBlock = parse_node_p{new parse_node(Token::block)};
    parse_node_p firstBlock = parse_node_p{new parse_node(Token::block)};
    parse_node_p lastBlock = parse_node_p{new parse_node(Token::block)};
    double radius = 0, xc = 0, zc = 0;
    for (auto &n : nodes){
        if (n->type() == Token::n_word){
            firstBlock->appendChild(n);
        } else if (n->type() != Token::parameter){
            newBlock->appendChild(n);
        } else {
            if (radius_to_coord){
                if (n->intValue() == param_circle_radius){
                    try {
                        double x1, z1, x2 = std::numeric_limits<double>::min(),
                            z2 = std::numeric_limits<double>::min();
                        x1 = transform_state.axis_positions['X'];
                        z1 = transform_state.axis_positions['Z'];
                        for (auto &n2 : nodes){
                            // TODO This all breaks if values are not simple numbers
                            if (n->type() == Token::parameter && n->intValue() == param_x && n->childCount()){
                                x2 = n->getChild(0)->doubleValue();
                            } else if (n->type() == Token::parameter && n->intValue() == param_z && n->childCount()){
                                z2 = n->getChild(0)->doubleValue();
                            } else if (n->type() == Token::parameter && n->intValue() == param_incremental_x
                                       && n->childCount()){
                                x2 = n->getChild(0)->doubleValue() + x1;
                            } else if (n->type() == Token::parameter && n->intValue() == param_incremental_z
                                       && n->childCount()){
                                z2 = n->getChild(0)->doubleValue() + z1;
                            } else if (n->type() == Token::parameter && n->intValue() == param_circle_radius
                                       && n->childCount()){
                                radius = n->getChild(0)->doubleValue();
                            }
                        }
                        double dx = x2 - x1;
                        double dz = z2 - z1;
                        double d = std::sqrt(dx*dx + dz*dz);
                        if (d > 2*radius)
                            throw std::domain_error("Circle impossible");
                        double h = std::sqrt(radius*radius - (d/2)*(d/2));
                        double ux = -dz / d;
                        double uz = dx / d;
                        double sign = (code == g_circular_interpolation_ccw) ? 1.0 : -1.0;
                        xc = ((x1 + x2) / 2) + sign * h * ux;
                        zc = ((z1 + z2) / 2) + sign * h * uz;
                    } catch (std::exception e){
                        newBlock->appendChild(n);
                        std::cerr << error(error::Generating, -1, -1, e.what()).to_string();
                    }
                } else {
                    newBlock->appendChild(n);
                }
            }
            if (coord_to_radius){
                double x1 = transform_state.axis_positions['X'];
                double z1 = transform_state.axis_positions['Z'];
                for (auto &n2 : nodes){
                    if (n->type() == Token::parameter && n->intValue() == param_circle_center_x && n->childCount()){
                        xc = n->getChild(0)->doubleValue();
                    } else if (n->type() == Token::parameter && n->intValue() == param_circle_center_z && n->childCount()){
                        zc = n->getChild(0)->doubleValue();
                    } else if (n->type() == Token::parameter && n->intValue() == param_circle_center_incr_x && n->childCount()){
                        xc = n->getChild(0)->doubleValue() + x1;
                    } else if (n->type() == Token::parameter && n->intValue() == param_circle_center_incr_x && n->childCount()){
                        zc = n->getChild(0)->doubleValue() + z1;
                    }
                }
                radius = std::sqrt((x1+xc)*(x1+xc) + (z1+zc)*(z1+zc));
            }
        }
    }
    auto appendChild = [&](int P, std::vector<parse_node_p> n){
        newBlock->appendChild(parse_node_p{new parse_node(Token::parameter, P, n)});
    };
    for (auto &n : nodes){
        if (incremental_to_absolute){
            switch (n->intValue()){
            case param_incremental_x:
                appendChild(param_x, n->children()); continue;
            case param_incremental_y:
                appendChild(param_y, n->children()); continue;
            case param_incremental_z:
                appendChild(param_z, n->children()); continue;
            case param_circle_center_incr_x:
                appendChild(param_circle_center_x, n->children()); continue;
            case param_circle_center_incr_z:
                appendChild(param_circle_center_z, n->children()); continue;
            }
        }
        if (coord_to_radius){
            switch (n->intValue()){
            case param_circle_center_x:
            case param_circle_center_z:
            case param_circle_center_incr_x:
            case param_circle_center_incr_z:
                if (radius){
                    appendChild(param_circle_radius, {parse_node_p(new parse_node(Token::num_float, radius))});
                    radius = 0;
                }
                continue;
            }
        }
        if (radius_to_coord){
            switch (n->intValue()){
            case param_circle_radius:
                appendChild(param_circle_center_x, {parse_node_p(new parse_node(Token::num_float, xc))});
                appendChild(param_circle_center_z, {parse_node_p(new parse_node(Token::num_float, zc))});
                continue;
            }
        }
        newBlock->appendChild(n);
    }
    if (incremental_to_absolute && transform_state.dimension_mode == machine_state::AbsoluteCoordinates){
        firstBlock->appendChild(parse_node_p{new parse_node(Token::g_word, g_incremental_dimension)});
        lastBlock->appendChild(parse_node_p{new parse_node(Token::g_word, g_absolute_dimension)});
    }
    if (firstBlock->childCount() == 1 && firstBlock->type() == Token::n_word){
        newBlock->prependChild(firstBlock->getChild(0));
    } else if (firstBlock->childCount()){
        newRoot->appendChild(firstBlock);
    }
    newRoot->appendChild(newBlock);
    if (lastBlock->childCount())
        newRoot->appendChild(lastBlock);
    root = newRoot;
    return true;
}

/*!
 * \brief Checks for feed or spindle rotation rates being specified as parameter word if unsupported
 *
 * If a feed rate or a spindle rotation rate has been supplied as a parameter word to a prepatory word,
 * e.g. "G1 X10 F20" or "G1 X10 S500", but the machine doesn't support changing these settings in the form
 * of parameter words then the parameter is transformed to the relevant G code for setting the value,
 * inserted before the current G code in the parse tree.
 *
 * \param code G code type currently being checked
 * \return returns true if a transformation has been applied, otherwise false
 */
bool parse_node_gen::feed_rpm(int code)
{
    auto nodes = root->children();
    for (auto n : nodes){
        if (n->type() == Token::parameter){
            int param_type = n->intValue();
            bool found = false;
            for (auto &m : {param->g[code].parameter_types, param->parameter.defaults}){
                for (auto &p : m){
                    if (p.second == param_type){
                        found = true;
                        break;
                    }
                }
            }
            if (found)
                continue;
            for (int type : {param_feed, param_rpm}){
                if (type != n->intValue())
                    continue;
                parse_node_p newRoot = parse_node_p{new parse_node(Token::list)};
                parse_node_p newBlock = parse_node_p{new parse_node(Token::block)};
                std::variant<int,double,std::string> value;
                for (auto n1 : nodes){
                    if (n1 != n){
                        newBlock->appendChild(n1);
                    } else {
                        switch (n1->getChild(0)->type()){
                        case Token::string:
                        case Token::num_literal:
                            value = n1->getChild(0)->stringValue(); break;
                        case Token::num_int:
                            value = n1->getChild(0)->intValue(); break;
                        case Token::num_float:
                            value = n1->getChild(0)->doubleValue();
                        }
                    }
                }
                auto addNodes = [&]<int T>(int P, int G){
                    newRoot->appendChildren({
                        parse_node_p{new parse_node(Token::block, {
                            parse_node_p{new parse_node(Token::g_word, G)},
                            parse_node_p{new parse_node(Token::parameter, P, {
                                parse_node_p(new parse_node(Token::num_int+T, std::get<T>(value)))
                            })}
                        })},
                        newBlock
                    });
                };
                int op;
                if (type == param_feed){
                    op = (transform_state.feed_mode == machine_state::FeedPerMinute)
                             ? g_feed_per_minute
                             : g_feed_per_revolution;
                } else {
                    op = (transform_state.rpm_mode == machine_state::RevolutionsPerMinute
                              ? g_revolutions_per_minute
                              : g_constant_surface_speed);
                }
                switch (value.index()){
                case 0: addNodes.operator()<0>(type, op); break;
                case 1: addNodes.operator()<1>(type, op); break;
                case 2: addNodes.operator()<2>(type, op);
                }
                root = newRoot;
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief Updates the current machine state based on G and M codes run
 *
 * Sets parameters such as coordinate input mode (absolute/incremental) and feed/spindle
 * rotation rate mode when these are changed by the supplied G or M code.
 *
 * The function will not update current tool position or current subsystem. This has to
 * be handled at the point of parsing.
 *
 * \param state machine_state object to make changes to
 * \param gcode G code type to update state based on
 * \param mcode M code type to update state based on
 */
void parse_node_gen::updateState(machine_state *state, int gcode, int mcode) const
{
    switch (gcode){ // Change machine state if needed
    case -1:
        break;
    case g_absolute_dimension:
        state->dimension_mode = machine_state::AbsoluteCoordinates; break;
    case g_incremental_dimension:
        state->dimension_mode = machine_state::IncrementalCoordinates; break;
    case g_revolutions_per_minute:
        state->rpm_mode = machine_state::RevolutionsPerMinute; break;
    case g_constant_surface_speed:
        state->rpm_mode = machine_state::ConstantSurfaceSpeed; break;
    case g_feed_per_minute:
        state->feed_mode = machine_state::FeedPerMinute; break;
    case g_feed_per_revolution:
        state->feed_mode = machine_state::FeedPerRevolution; break;
    }
    switch (mcode){
    case -1:
        return;
    }
}

}

