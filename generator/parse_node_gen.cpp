#include <iostream>
#include <sstream>
#include "parse_node_gen.h"
#include "../brands/parameters.h"
#include "../parser/parse_node.h"
#include "../tokens/tokens.h"
#include "../errors/errors.h"

namespace NCParser {

std::pair<std::string, parse_node_gen::machine_state> NCParser::parse_node_gen::generate(parse_node_p block, machine_state machineState)
{
    root = block;
    state = machineState;
    std::pair<std::string, parse_node_gen::machine_state> returnValue;
    do { // Run through the generator, until we have a parse tree that can be used and returns an answer
        returnValue = generate_internal();
    } while (rerun);
    return returnValue;
}

std::pair<std::string, parse_node_gen::machine_state> parse_node_gen::generate_internal()
{
    rerun = false; // Expect that tree is useable

    // If this node is only an expression, return it
    if (!root->childCount() || (root->type() != Token::block && root->type() != Token::list)){
        return {expr(root), state};
    }
    std::stringstream ss;

    // Check for, and complete, the first needed transformations, if any
    for (auto &n : root->children()){
        if (n->type() == Token::g_word){
            rerun = g(n->intValue());
            if (rerun){
                return {};
            }
        }
    }

    // If node is a list of blocks, run subprocesses for each block
    if (root->type() == Token::list){
        for (auto &b : root->children()){
            auto r = parse_node_gen(param).generate(b, state);
            ss << r.first;
            state = r.second;
        }
        return {ss.str(), state};
    }

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
                // TODO Handle non-existant G-ocdes
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
        {
            auto r = generate(n, state);
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

std::string parse_node_gen::expr(parse_node_p n)
{
    static auto get_func = [](int v, std::map<std::string,int> * m) -> std::string{
        for (auto &i : *m){
            if (i.second == v){
                return i.first;
            }
        }
        // TODO Perhaps simulate missing, if needed for some machines
        std::cerr << (error{error::Generating, -1, -1, "Function not translateable"}).to_string();
        return "";
    };

    std::stringstream ss;
    switch (n->type()){
    // Binary
    case Token::equals:
        if (n->childCount() > 1){
            ss << expr(n->getChild(0)) << " = " << expr(n->getChild(1)); break;
        } else {
            ss << " = " << expr(n->getChild(0)); break;
        }
    case Token::star:
        ss << "[" << expr(n->getChild(0)) << " * " << expr(n->getChild(1)) << "]"; break;
    case Token::plus:
        ss << "[" << expr(n->getChild(0)) << " + " << expr(n->getChild(1)) << "]"; break;
    case Token::minus:
        ss << "[" << expr(n->getChild(0)) << " - " << expr(n->getChild(1)) << "]"; break;
    case Token::slash:
        ss << "[" << expr(n->getChild(0)) << " / " << expr(n->getChild(1)) << "]"; break;
    case Token::binary_functions:
        ss << "[" << expr(n->getChild(0)) << " " << get_func(n->intValue(), &(param->functions.binary))
            << " " << expr(n->getChild(1)) << "]"; break;

    // Values
    case Token::num_literal:
        ss << n->stringValue(); break;
    case Token::num_int:
        ss << std::to_string(n->intValue()); break;
    case Token::num_float:
        // TODO Implement better float handling, to fit different machine parsing requirements
        ss << std::to_string(n->doubleValue()); break;
    case Token::string:
        ss << n->stringValue(); break;
    case Token::variable:
    {
        // TODO Implement more extensive variable handling, this will break
        if (param->variables.variable_marker.size()){
            for (char c : param->variables.variable_marker){
                ss << c << n->stringValue();
                break;
            }
        } else {
            for (auto i : param->parameter.defaults){
                if (i.second == param_variable){
                    ss << i.first << n->stringValue();
                    break;
                }
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
        ss << "."; break;
    case Token::percent:
        ss << "%"; break;
    }
    return ss.str();
}

bool parse_node_gen::g(int code)
{
    if (transform_coordinates(code))
        return true;
    if (feed_rpm(code))
        return true;
    switch ((GCodesAvailable)code){
    case g_circular_interpolation_ccw: return g_circular_interpolation_ccw_func();
    case g_circular_interpolation_cw: return g_circular_interpolation_cw_func();
    case g_linear_interpolation: return g_linear_interpolation_func();
    case g_linear_interpolation_with_collision_stop: return g_linear_interpolation_with_collision_stop_func();
    case g_linear_interpolation_with_feed_reduction: return g_linear_interpolation_with_feed_reduction_func();
    case g_rapid_positioning: return g_rapid_positioning_func();
    case g_auto_geometry_function: break;
    case g_absolute_dimension: break;
    case g_use_subsystem_a: break;
    case g_use_subsystem_b: break;
    case g_use_subsystem_c: break;
    case g_use_subsystem_d: break;
    case g_bevel_cycle_against_spindle: break;
    case g_bevel_cycle_towards_spindle: break;
    case g_chuck_transfer_under_torque: break;
    case g_circular_interpolation_ccw_milling_on_turning: break;
    case g_circular_interpolation_cw_milling_on_turning: break;
    case g_constant_surface_speed: break;
    case g_continuous_path_mode: break;
    case g_coordinate_system_set: break;
    case g_coordinate_cartesian_to_polar: break;
    case g_coordinate_cartesian_to_polar_off: break;
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
    case g_load_subprogram: break;
    case g_milling_interpolation_off: break;
    case g_milling_interpolation_on: break;
    case g_milling_line_end_face: break;
    case g_milling_curve_cw_end_face: break;
    case g_milling_curve_ccw_end_face: break;
    case g_parabolic_interpolation: break;
    case g_preload_registers: break;
    case g_r_point_return_fixed: break;
    case g_rapid_direct_to_reference: break;
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
    case g_wedge_cut_cycle_lengthwise: break;
    case g_xy_plane_selection: break;
    case g_xz_cylindrical_plane_selection: break;
    case g_yz_cylindrical_plane_selection: break;
    case g_yz_plane_selection: break;
    case g_zero_set: break;
    case g_zero_shift: break;
    case g_zx_plane_selection: break;
    case g_zy_plane_selection: break;
    case g_turning_mode: break;
    case g_c_axis_mode: break;
    case g_y_axis_mode: break;
    case g_dwell: break;
    }
    return false;
}

bool parse_node_gen::transform_coordinates(int code)
{
    // TODO Handle X/Y/Z coordinates while in incremental mode if machine has U/W/V
    auto nodes = root->children();
    bool incremental_to_absolute = false;
    bool radius_to_coord = false;
    bool coord_to_radius = false;
    for (auto n : nodes){
        if (n->type() == Token::parameter){
            int param_type = n->intValue();
            bool found = false;
            for (auto p : param->g[code].parameter_types){
                if (p.second == param_type){
                    found = true;
                }
            }
            if (!found){
                for (auto p : param->parameter.defaults){
                    if (p.second == param_type){
                        found = true;
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
    if (!incremental_to_absolute
        && !radius_to_coord
        && !coord_to_radius)
        return false;
    parse_node_p newRoot = parse_node_p{new parse_node(Token::list)};
    parse_node_p newBlock = parse_node_p{new parse_node(Token::block)};
    parse_node_p firstBlock = parse_node_p{new parse_node(Token::block)};
    for (auto n : nodes){
        if (n->type() == Token::n_word){
            firstBlock->appendChild(n);
        } else if (n->type() != Token::parameter){
            newBlock->appendChild(n);
        } else {
            if (incremental_to_absolute){
                switch (n->intValue()){
                case param_incremental_x:
                    newBlock->appendChild(parse_node_p{new parse_node(Token::parameter,
                                                                      param_x, n->children())});
                    break;
                case param_incremental_y:
                    newBlock->appendChild(parse_node_p{new parse_node(Token::parameter,
                                                                      param_y, n->children())});
                    break;
                case param_incremental_z:
                    newBlock->appendChild(parse_node_p{new parse_node(Token::parameter,
                                                                      param_z, n->children())});
                    break;
                case param_circle_center_incr_x:
                    newBlock->appendChild(parse_node_p{new parse_node(Token::parameter,
                                                                      param_circle_center_x, n->children())});
                    break;
                case param_circle_center_incr_z:
                    newBlock->appendChild(parse_node_p{new parse_node(Token::parameter,
                                                                      param_circle_center_incr_z, n->children())});
                    break;
                default:
                    newBlock->appendChild(n);
                }
            }
            if (radius_to_coord){
                // TODO Implement
            }
            if (coord_to_radius){
                // TODO Implement
            }
        }
    }
    if (incremental_to_absolute){
        firstBlock->appendChild(parse_node_p{new parse_node(Token::g_word, g_incremental_dimension)});
        newRoot->appendChildren({
            firstBlock,
            newBlock,
            parse_node_p{new parse_node(Token::block, -1, {
                parse_node_p{new parse_node(Token::g_word, g_absolute_dimension)}
            })}
        });
    } else {
        if (firstBlock->childCount())
            newBlock->prependChild(firstBlock->getChild(0));
        newRoot->appendChild(newBlock);
    }
    root = newRoot;
    return true;
}

bool parse_node_gen::feed_rpm(int code)
{
    auto nodes = root->children();
    for (auto n : nodes){
        if (n->type() == Token::parameter){
            int param_type = n->intValue();
            bool found = false;
            for (auto p : param->g[code].parameter_types){
                if (p.second == param_type){
                    found = true;
                }
            }
            if (!found){
                for (auto p : param->parameter.defaults){
                    if (p.second == param_type){
                        found = true;
                    }
                }
            }
            if (n->intValue() == param_feed && !found){
                parse_node_p newRoot = parse_node_p{new parse_node(Token::list)};
                parse_node_p newBlock = parse_node_p{new parse_node(Token::block)};
            } else if (n->intValue() == param_rpm && !found){
                parse_node_p newRoot = parse_node_p{new parse_node(Token::list)};
                parse_node_p newBlock = parse_node_p{new parse_node(Token::block)};
                std::variant<int,double,std::string> value;
                int valueType;
                for (auto n1 : nodes){
                    if (n1 != n){
                        newBlock->appendChild(n1);
                    } else {
                        valueType = n1->getChild(0)->type();
                        if (valueType == Token::string || valueType == Token::num_literal){
                            value = n1->getChild(0)->stringValue();
                        } else if (valueType == Token::num_int){
                            value = n1->getChild(0)->intValue();
                        } else {
                            value = n1->getChild(0)->doubleValue();
                        }
                    }
                }
                switch (value.index()){
                case 0:
                    newRoot->appendChildren({
                        parse_node_p{new parse_node(Token::block, {
                            parse_node_p{new parse_node(Token::g_word, g_revolutions_per_minute)},
                            parse_node_p{new parse_node(Token::parameter, param_rpm, {
                                parse_node_p(new parse_node(valueType, std::get<0>(value)))
                            })}
                        })},
                        newBlock
                    }); break;
                case 1:
                    newRoot->appendChildren({
                          parse_node_p{new parse_node(Token::block, {
                            parse_node_p{new parse_node(Token::g_word, g_revolutions_per_minute)},
                            parse_node_p{new parse_node(Token::parameter, param_rpm, {
                                parse_node_p(new parse_node(valueType, std::get<1>(value)))
                            })}
                        })},
                        newBlock
                    }); break;
                default:
                    newRoot->appendChildren({
                        parse_node_p{new parse_node(Token::block, {
                            parse_node_p{new parse_node(Token::g_word, g_revolutions_per_minute)},
                            parse_node_p{new parse_node(Token::parameter, param_rpm, {
                                parse_node_p(new parse_node(valueType, std::get<2>(value)))
                            })}
                        })},
                        newBlock
                    }); break;
                }
                root = newRoot;
                return true;
            }
        }
    }
    return false;
}

}

