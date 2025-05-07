#include <cctype>
#include <string>
#include <map>
#include "lexer.h"
#include "../tokens/tokens.h"

namespace NCParser {

const std::set<int> lexer::g_codes = {GCodes::rapid_positioning, GCodes::linear_interpolation, GCodes::circular_interpolation_cw,
                                      GCodes::circular_interpolation_ccw, GCodes::dwell, GCodes::parabolic_interpolation, GCodes::exact_stop, GCodes::xy_plane_selection,
                                      GCodes::zx_plane_selection, GCodes::zy_plane_selection, GCodes::thread_cutting_constant_lead, GCodes::thread_cutting_increasing_lead,
                                      GCodes::thread_cutting_decreasing_lead, GCodes::cutter_compensation_cancel, GCodes::cutter_compensation_left,
                                      GCodes::cutter_compensation_right, GCodes::tool_offset_positive, GCodes::tool_offset_negative, GCodes::dimension_shift_cancel,
                                      GCodes::zero_shift, GCodes::tapping, GCodes::continuous_path_mode, GCodes::dimension_input_inch, GCodes::dimension_input_metric,
                                      GCodes::home_position, GCodes::fixed_cycle_cancel, GCodes::fixed_cycle_81, GCodes::fixed_cycle_82, GCodes::fixed_cycle_83, GCodes::fixed_cycle_84,
                                      GCodes::fixed_cycle_85, GCodes::fixed_cycle_86, GCodes::fixed_cycle_87, GCodes::fixed_cycle_88, GCodes::fixed_cycle_89, GCodes::absolute_dimension,
                                      GCodes::incremental_dimension, GCodes::preload_registers, GCodes::inverse_time, GCodes::feed_per_minute, GCodes::feed_per_revolution,
                                      GCodes::constant_surface_speed, GCodes::revolutions_per_minute};

const std::set<int> lexer::m_codes = {MCodes::program_stop, MCodes::optional_stop, MCodes::end_of_program, MCodes::spindle_cw, MCodes::spindle_ccw,
                                      MCodes::spindle_off, MCodes::tool_change, MCodes::m07, MCodes::m08, MCodes::m09, MCodes::clamp_workpiece, MCodes::unclamp_workpiece,
                                      MCodes::end_of_data, MCodes::cancel_m49, MCodes::bypass_override, MCodes::workpiece_change};

lexer::lexer() {}

void lexer::init(std::string text, MachineParameters param)
{
    this->param = param;
    this->text = text;
    pos = 0;
    m_line = 0;
}

int lexer::next()
{
    // Helper to peek forward
    static auto peek = [this]() -> char {
        int p = pos;
        while (text.length() > p){
            if (text.at(p) == ' ' || text.at(p) == '\t' || text.at(p) == '\n'){
                p++;
            } else {
                return text.at(p);
            }
        }
        return EOF;
    };

    // Grab the next group of numeric characters
    static auto grabInt = [&]() -> std::string {
        int start = pos;
        while (std::isdigit(peek())){
            pos++;
        }
        return text.substr(start, pos-start);
    };

    // Parse what G- or M-code we have
    static auto getCode = [&](int token) -> int {
        if (!std::isdigit(peek())){
            return Token::unknown_code;
        }
        pos++;
        std::string num = grabInt();
        iValue = std::stoi(num);
        return token;
    };

    // Most single characters will correspond to a single token
    static std::map<char,int> charMap = {
        {'%', Token::percent}, {')', Token::rightparen},
        {'*', Token::star}, {'+', Token::plus}, {',', Token::comma}, {'-', Token::minus},
        {'.', Token::period}, {'/', Token::slash}, {':', Token::colon}, {'=', Token::equals},
        {'[', Token::leftsquare}, {']', Token::rightsquare}, {'A', Token::a_word}, {'B', Token::b_word},
        {'C', Token::c_word}, {'D', Token::d_word}, {'E', Token::e_word}, {'F', Token::f_word},
        {'I', Token::i_word}, {'J', Token::j_word}, {'K', Token::k_word},
        {'O', Token::o_word}, {'P', Token::p_word}, {'Q', Token::q_word}, {'R', Token::r_word},
        {'S', Token::s_word}, {'T', Token::t_word}, {'U', Token::u_word}, {'V', Token::v_word},
        {'W', Token::w_word}, {'X', Token::x_word}, {'Y', Token::y_word}, {'Z', Token::z_word}
    };

    while (true){
        if (text.length() <= pos){
            return Token::done;
        }
        char next = std::toupper(text.at(pos));
        pos++;
        switch (next){
        case 'G':
            return getCode(Token::g_word);
        case 'L':
            if (param.L.is_variable){
                if (std::isdigit(peek())){
                    iValue = std::stoi(grabInt());
                    return Token::variable;
                } else {
                    continue;
                }
            } else {
                return Token::l_word;
            }
        case 'M':
            return getCode(Token::m_word);
        case 'N':
        {
            if (!std::isdigit(peek()))
                continue;
            iValue = std::stoi(grabInt());
            return Token::n_word;
        }
        case 'O':
            if (param.O.is_prg_name){
                if (std::isdigit(peek())){
                    iValue = std::stoi(grabInt());
                } else {
                    iValue = 0;
                }
                return Token::prg_name;
            } else {
                return Token::o_word;
            }
        case 0: case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8: case 9:
            {
                /*
                 * TODO: The standard defines additional possible ways to write numbers.
                 *       Those should also be implemented.
                 */
                std::string num = grabInt();
                if (peek() == '.'){
                    pos++;
                    num += '.' + grabInt();
                    fValue = std::stod(num);
                    return Token::num_float;
                } else {
                    iValue = std::stoi(num);
                    return Token::num_int;
                }
            }
        case '(':
            if (param.comments.use_parenthesis){
                int start = pos;
                while (text.at(pos) != ')' && text.at(pos) != '\n'){
                    pos++;
                }
                if (pos <= start+2){
                    continue;
                }
                sValue = text.substr(start+1, pos-(start+1));
                return Token::comment;
            } else {
                return Token::leftparen;
            }
        case ';':
            if (param.comments.use_semicolon){
                int start = pos;
                while (text.at(pos) != '\n'){
                    pos++;
                }
                if (pos <= start+1){
                    continue;
                }
                sValue = text.substr(start+1, pos-1);
                return Token::comment;
            } else {
                return Token::semicolon;
            }
        case '\n':
            m_line++;
            return Token::newline;
        default:
            if (charMap.contains(next)){
                return charMap.at(next);
            } else {
                /* Right now we just ignore characters not specified as valid in ISO 6983-1:2009,
                 * or in an implemented machine/manufacturer specification.
                 * We might want to change this in the future.
                 */
                continue;
            }
        }
    }
    return Token::error;
}

};
