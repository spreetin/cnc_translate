#include "parser.h"
#include "../brands/machine_definitions.h"
#include <iostream>
#include <memory>
#include "parse_node.h"
#include "../tokens/tokens.h"

namespace NCParser {

parser::parser(Manufacturers manufacturer, std::string_view machine)
{
    m_lexer = std::unique_ptr<lexer>(new lexer());
    init(manufacturer, machine);
}

parser::parser(MachineParameters params)
{
    m_lexer = std::unique_ptr<lexer>(new lexer());
    init(params);
}

void parser::init(Manufacturers manufacturer, std::string_view machine)
{
    this->manufacturer = manufacturer;
    this->machine = machine;
    auto topLayer = std::unique_ptr<definition_base>(MachineDefinitions::getDefinition(manufacturer, machine));
    if (topLayer){
        param = topLayer->getParameters();
    }
    init(param);
}

void parser::init(MachineParameters params)
{
    param = params;
    allowed_multiletter.clear();
    for (auto &item : param.functions.unary){
        allowed_multiletter.insert(item.first);
    }
    for (auto &item : param.functions.binary){
        allowed_multiletter.insert(item.first);
    }
    for (auto &item : param.functions.block){
        allowed_multiletter.insert(item.first);
    }
    for (auto &item : param.g){
        if (item.second.word.index()){
            for (auto &num : std::get<1>(item.second.word)){
                active_g_numbers[num] = item.first;
            }
        } else {
            active_g_numbers[std::get<int>(item.second.word)] = item.first;
        }
    }
    for (auto &item : param.m){
        active_m_numbers[item.second.num] = item.first;
    }
}

bool parser::parse(std::string text)
{
    m_lexer->init(text, allowed_multiletter);
    m_result.clear();
    start();
    if (m_result.size()){
        return true;
    } else {
        return false;
    }
}

void parser::match(int code)
{
    if (next != code){
        error err {error::Parsing, m_lexer->line(), m_lexer->pos(),
                "Faulty match: \"" + std::to_string(code)};
        errors.push_back(err);
        std::cerr << err.to_string() << std::endl;
    }
    if (next != Token::done){
        next = m_lexer->next();
        if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
            errors.push_back(m_lexer->last_error());
            std::cerr << m_lexer->last_error().to_string() << std::endl;
            match(next);
        }
    }
}

void parser::start()
{
    next = m_lexer->next();
    if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
        errors.push_back(m_lexer->last_error());
        std::cerr << m_lexer->last_error().to_string() << std::endl;
    }
    //if (next == Token::percent)
    //    match(Token::percent);
    m_result = list();
}

std::vector<parse_node_p> parser::list()
{
    std::vector<parse_node_p> blocks;
    while (next != Token::done){
        if (next == Token::empty_line){
            blocks.push_back(std::make_shared<parse_node>(Token::block,
                std::vector<parse_node_p>{std::make_shared<parse_node>(Token::empty_line)}
            ));
            match(Token::empty_line);
        } else {
            auto b = block();
            if (b)
                blocks.push_back(b);
        }
    }
    return blocks;
}

parse_node_p parser::block()
{
    /*
     * Order defined in standard as N -> G -> dimensions -> I/J/K -> feed function -> spindle speed -> T -> M
     * We parse blocks with the assumption that they follow the most important parts of that definition:
     * N -> G -> parameters -> M
     * The order of parameters are accepted even when they would be wrong according to the standard.
     */
    auto rowItem = std::make_shared<parse_node>(Token::block);
    if (next == Token::slash){
        match(Token::slash);
        rowItem->appendChild(std::make_shared<parse_node>(Token::block_delete));
    }
    if (next == Token::n_word){
        std::string blockNum = m_lexer->string_value();
        match(Token::n_word);
        rowItem->appendChild(std::make_shared<parse_node>(Token::n_word, blockNum));
    }
    if (next == Token::percent){
        match(Token::percent);
        rowItem->appendChild(std::make_shared<parse_node>(Token::percent));
    }
    if (param.parameter.defaults[next] == param_prg_name){
        match(next);
        rowItem->appendChild(std::make_shared<parse_node>(Token::prg_name, std::vector<parse_node_p>{expr()}));
    }
    if (next == param.subsystem.symbol){
        if (next == Token::g_word){
            int code = m_lexer->int_value();
            if (param.g.contains(g_use_subsystem_a)
                        && std::get<int>(param.g[g_use_subsystem_a].word) == code){
                match(Token::g_word);
                rowItem->appendChild(std::make_shared<parse_node>(Token::subsystem_select, 1));
            } else  if (param.g.contains(g_use_subsystem_b)
                        && std::get<int>(param.g[g_use_subsystem_b].word) == code){
                match(Token::g_word);
                rowItem->appendChild(std::make_shared<parse_node>(Token::subsystem_select, 2));
            } else  if (param.g.contains(g_use_subsystem_c)
                       && std::get<int>(param.g[g_use_subsystem_c].word) == code){
                match(Token::g_word);
                rowItem->appendChild(std::make_shared<parse_node>(Token::subsystem_select, 3));
            } else  if (param.g.contains(g_use_subsystem_d)
                       && std::get<int>(param.g[g_use_subsystem_d].word) == code){
                match(Token::g_word);
                rowItem->appendChild(std::make_shared<parse_node>(Token::subsystem_select, 4));
            }
        } else {
            match(next);
            if (next == Token::num_literal){
                int num = m_lexer->int_value();
                match(Token::num_literal);
                rowItem->appendChild(std::make_shared<parse_node>(Token::subsystem_select, num));
            }
        }
    }
    if (next == Token::multi_letter){
        // Such as WHILE-loops
        std::string ml = m_lexer->string_value();
        if (param.functions.block.contains(ml)){
            auto block_definition = param.functions.block.at(ml);
            std::vector<parse_node_p> blocks;
            match(Token::multi_letter);
            if (next != Token::done) {
                blocks.push_back(expr());
            } else {
                return rowItem;
            }
            while (next != Token::done){
                if (next == Token::multi_letter && std::toupper(block_definition.end_token) == std::toupper(m_lexer->string_value())){
                    match(Token::multi_letter);
                    break;
                }
                blocks.push_back(block());
            }
            rowItem->appendChild(std::make_shared<parse_node>(Token::block_function,
                                                                            block_definition.type, blocks));
        } else {
            match(Token::multi_letter);
        }
    }
    if (param.variables.variable_marker.contains(next)){
        if (auto n = assignment()){
            rowItem->appendChild(n);
        }
    }
    if (next == Token::g_word){
        while (next == Token::g_word){
            rowItem->appendChildren(g());
        }
    } else {
        int type = active_g_numbers[last_prepatory_word];
        if (param.g[type].parameters.contains(next)){
            rowItem->appendChildren(g(true));
        }
    }
    while (next == Token::m_word){
        int code = m_lexer->int_value();
        match(Token::m_word);
        if (active_m_numbers.contains(code)){
            rowItem->appendChild(std::make_shared<parse_node>(Token::m_word, active_m_numbers.at(code)));
        }
    }
    if ((param.comments.use_parenthesis && next == Token::leftparen)
        || (param.comments.use_semicolon && next == Token::semicolon)){
        rowItem->appendChild(comment());
    }
    if (next == Token::newline){
        match(Token::newline);
        return rowItem;
    } else if (next == Token::empty_line){
        return rowItem;
    } else if (next != Token::done){
        /*
         * These are pieces of data in the block that shouldn't be there.
         * We will not parse them, but will preserve them as a data node, for debugging purposes.
         * It is also possible that valid data ends up here, if the order of operations is wrong.
         * I.e. M-codes are written before G-codes.
         */
        m_lexer->finish_line();
        if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
            errors.push_back(m_lexer->last_error());
            std::cerr << m_lexer->last_error().to_string() << std::endl;
        }
        rowItem->appendChild(std::make_shared<parse_node>(Token::left_over_data, m_lexer->string_value()));
        next = m_lexer->next();
    }
    return rowItem;
}

parse_node_p parser::expr()
{
    auto p = term();
    return moreterms(p);
}

parse_node_p parser::assignment()
{
    auto var = variable();
    if (next == Token::equals){
        match(Token::equals);
        return std::make_shared<parse_node>(Token::equals, std::vector<parse_node_p>{var, expr()});
    }
    return var;
}

parse_node_p parser::variable()
{
    // TODO Handle more types of variables
    match(next);
    if (next != Token::num_literal)
        return std::make_shared<parse_node>(Token::variable, -1);
    int val = m_lexer->int_value();
    match(Token::num_literal);
    return std::make_shared<parse_node>(Token::variable, val);
}

parse_node_p parser::number()
{
    /*
     * TODO: We need to handle other ways of writing numbers that are valid, i.e. floating point
     *       numbers defined with fixed period position. These will just be parsed as int right now.
     */
    std::string first_part = m_lexer->string_value();
    match(Token::num_literal);
    if (first_part.size() && first_part.at(0) == '0'){
        if (next == Token::period){
            match(Token::period);
            first_part += ".";
            if (next == Token::num_literal){
                std::string second_part = m_lexer->string_value();
                first_part += second_part;
                match(Token::num_literal);
            }
        }
        return std::make_shared<parse_node>(Token::num_literal, first_part);
    } else if (next == Token::period){
        match(Token::period);
        if (next != Token::num_literal)
            return std::make_shared<parse_node>(Token::num_int, std::stoi(first_part));
        std::string second_part = m_lexer->string_value();
        match(Token::num_literal);
        double value = stod(first_part + '.' + second_part);
        return std::make_shared<parse_node>(Token::num_float, value);
    } else {
        try {
            return std::make_shared<parse_node>(Token::num_int, std::stoi(first_part));
        } catch (std::invalid_argument){
            error err {error::Parsing, m_lexer->line(), m_lexer->pos(),
                      "Number parse error on string: \"" + first_part + "\""};
            errors.push_back(err);
            std::cerr << err.to_string() << std::endl;
        } catch (std::out_of_range){
            error err {error::Parsing, m_lexer->line(), m_lexer->pos(),
                      "Number parse error on string: \"" + first_part + "\""};
            errors.push_back(err);
            std::cerr << err.to_string() << std::endl;
        }
        return nullptr;
    }
}

std::vector<parse_node_p> parser::g(bool continuingWord)
{
    int code;
    if (!continuingWord){
        code = m_lexer->int_value();
        match(Token::g_word);
    } else {
        code = last_prepatory_word;
    }
    std::vector<parse_node_p> nodes;
    if (active_g_numbers.contains(code)){
        if (code < 5 && !continuingWord){
            last_prepatory_word = code;
        }
        int g_type = active_g_numbers.at(code);
        auto def = param.g[g_type];
        nodes.push_back(std::make_shared<parse_node>(Token::g_word, g_type));
        while (def.parameters.contains(next)){
            int param_type;
            std::vector<int> sub_types;
            if (def.parameter_types.contains(next)){
                param_type = def.parameter_types.at(next);
                if (param.parameter.subtypes.contains(next)){
                    sub_types = param.parameter.subtypes.at(next);
                }
            } else if (param.parameter.defaults.contains(next)){
                param_type = param.parameter.defaults.at(next);
            } else { // Invalid word
                param_type = next;
            }
            match(next);
            parse_node_p e = grabParameterData(sub_types);
            nodes.push_back(std::make_shared<parse_node>(Token::parameter, param_type, std::vector<parse_node_p>{e}));
        }
    }
    return nodes;
}

parse_node_p parser::grabParameterData(std::vector<int> sub_types)
{
    parse_node_p l;
    if (next == Token::equals){
        match(Token::equals);
        l = expr();
        return std::make_shared<parse_node>(Token::equals, std::vector<parse_node_p>{l});
    } else {
        l = expr();
        if (sub_types.size() > 1 && next == Token::equals){
            match(Token::equals);
            parse_node_p r = expr();
            return std::make_shared<parse_node>(Token::equals, std::vector<parse_node_p>{l, r});
        } else {
            return l;
        }
    }
}

parse_node_p parser::comment()
{
    char init_char = next;
    char end_char = ')';
    if (init_char == '(')
        end_char = ')';
    m_lexer->finish_comment(end_char);
    if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
        errors.push_back(m_lexer->last_error());
        std::cerr << m_lexer->last_error().to_string() << std::endl;
    }
    std::string comment_text = m_lexer->string_value();
    next = m_lexer->next();
    if (next == end_char)
        match(next);
    return std::make_shared<parse_node>(Token::comment, comment_text);
}

parse_node_p parser::term()
{
    auto p = factor();
    return morefactors(p);
}

parse_node_p parser::moreterms(parse_node_p left)
{
    static std::set<int> ops = {Token::plus, Token::minus};
    if (ops.contains(next)){
        int op = next;
        match(op);
        auto p = std::make_shared<parse_node>(op, std::vector<parse_node_p>{left, term()});
        return moreterms(p);
    } else {
        return left;
    }
}

parse_node_p parser::factor()
{
    auto p = func();
    return morefuncs(p);
}

parse_node_p parser::morefactors(parse_node_p left)
{
    static std::set<int> ops = {Token::star, Token::slash};
    if (ops.contains(next)){
        int op = next;
        match(op);
        auto p = std::make_shared<parse_node>(op, std::vector<parse_node_p>{left, factor()});
        return morefactors(p);
    } else {
        return left;
    }
}

parse_node_p parser::func()
{
    if (next == Token::plus || next == Token::minus){
        auto t = next;
        std::string n;
        int multiplier = 1;
        if (t == Token::plus){
            n = "+";
            match (Token::plus);
        } else {
            n = "-";
            multiplier = -1;
            match (Token::minus);
        }
        auto e = expr();
        if (e && e->type() == Token::num_int){
            e->setValue(multiplier * e->intValue());
            return e;
        } else if (e && e->type() == Token::num_float){
            e->setValue(multiplier * e->doubleValue());
            return e;
        } else if(e && e->type() == Token::num_literal){
            e->setValue(n + e->stringValue());
            e->setType(Token::num_literal);
            return e;
        } else {
            return std::make_shared<parse_node>(t, std::vector<parse_node_p>{e});
        }
    } else if (next == Token::num_literal){
        return morefuncs(number());
    } else if (next == Token::string){
        return morefuncs(std::make_shared<parse_node>(Token::string, m_lexer->string_value()));
    } else if (next == Token::leftsquare){
        match(Token::leftsquare);
        auto p = expr();
        match(Token::rightsquare);
        return morefuncs(p);
    } else if (next == Token::multi_letter){
        std::string t = m_lexer->string_value();
        if (param.functions.unary.contains(t)){
            match(Token::multi_letter);
            return morefuncs(std::make_shared<parse_node>(Token::unary_function, param.functions.unary[t], std::vector<parse_node_p>{expr()}));
        }
    } else if (param.variables.variable_marker.contains(next)){
        // TODO handle more types of variables
        return morefuncs(variable());
    }
    error err;
    if (m_lexer->hasValue()){
        err = {error::Parsing, m_lexer->line(), m_lexer->pos(),
                  "Expression non-parseable. Token: " + std::to_string(next) + " Variable: "
                      + m_lexer->string_value()};
    } else {
        err = {error::Parsing, m_lexer->line(), m_lexer->pos(),
                  "Expression non-parseable. Token: " + std::to_string(next) + " Variable: N/A"};
    }
    errors.push_back(err);
    std::cerr << err.to_string() << std::endl;
    return nullptr;
}

parse_node_p parser::morefuncs(parse_node_p left)
{
    if (next == Token::multi_letter){
        std::string t = m_lexer->string_value();
        if (param.functions.binary.contains(t)){
            match(Token::multi_letter);
            return std::make_shared<parse_node>(Token::binary_functions, param.functions.binary[t], std::vector<parse_node_p>{left, expr()});
        }
    }
    return left;
}

};
