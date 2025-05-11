#include <iostream>
#include <sstream>
#include "parse_node_gen.h"
#include "../brands/parameters.h"
#include "../parser/parse_node.h"
#include "../tokens/tokens.h"
#include "../errors/errors.h"

namespace NCParser {

std::string NCParser::parse_node_gen::generate(std::shared_ptr<parse_node>block, MachineParameters *param)
{
    if (!block->childCount()){
        return expr(block, param);
    }
    std::stringstream ss;
    int g_code = -1;
    for (auto n : block->children()){
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
        } break;
        case Token::m_word:
            if (param->m.contains(n->intValue())){
                ss << "M" + std::to_string(param->m[n->intValue()].num) << " ";
            } else {
                std::cerr << (error{error::Generating, -1, -1, "No translation found for M-code with ID: " + n->stringValue()}).to_string();
            }
            break;
        case Token::n_word:
            ss << "N" << n->stringValue() << " ";
            break;
        case Token::left_over_data:
            std::cerr << (error{}).to_string();
            break;
        case Token::block_function:
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
                for (auto i : param->parameter_defaults){
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
            ss << expr(n->getChild(0), param) << " ";
        } break;
        case Token::prg_name:
            for (auto i : param->parameter_defaults){
                if (i.second == param_prg_name){
                    ss << i.first << n->stringValue();
                    break;
                }
            }
            break;
        case Token::variable:
            ss << expr(n, param); break;
        case Token::comment:
            if (param->comments.use_parenthesis){
                ss << "(" << n->stringValue() << ")";
            } else if (param->comments.use_semicolon){
                ss << ";" << n->stringValue();
            } else {
                std::cerr << (error{error::Generating, -1, -1, "No comment format enabled, comment ignored."}).to_string();
            }
        case Token::block:
            ss << generate(n, param); break;
        }
    }
    ss << "\n";
    return ss.str();
}

std::string NCParser::parse_node_gen::expr(std::shared_ptr<parse_node>n, MachineParameters *param)
{
    static auto get_func = [](int v, std::map<std::string,int> * m) -> std::string{
        for (auto i : *m){
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
        ss << expr(n->getChild(0), param) << " = " << expr(n->getChild(1), param); break;
    case Token::star:
        ss << "[" << expr(n->getChild(0), param) << " * " << expr(n->getChild(1), param) << "]"; break;
    case Token::plus:
        ss << "[" << expr(n->getChild(0), param) << " + " << expr(n->getChild(1), param) << "]"; break;
    case Token::minus:
        ss << "[" << expr(n->getChild(0), param) << " - " << expr(n->getChild(1), param) << "]"; break;
    case Token::slash:
        ss << "[" << expr(n->getChild(0), param) << " / " << expr(n->getChild(1), param) << "]"; break;
    case Token::binary_functions:
        ss << "[" << expr(n->getChild(0), param) << " " << get_func(n->intValue(), &(param->functions.binary))
            << " " << expr(n->getChild(1), param) << "]"; break;

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
            for (auto i : param->parameter_defaults){
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
        ss << get_func(n->intValue(), &(param->functions.unary)) << "[" << expr(n->getChild(0), param) << "]"; break;
    // Unclear
    case Token::multary_functions:
    case Token::period:
        break;
    }
    return ss.str();
}

std::string pnw::to_string() const
{
    return parse_node_gen::generate(n, param);
}

std::ostream& operator<<(std::ostream& os, const pnw &pnw){
    os << pnw.to_string();
    return os;
}

}

