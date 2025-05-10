#include "parser.h"
#include "../brands/machine_definitions.h"
#include <iostream>

namespace NCParser {

parser::parser(Controllers controller, Manufacturers manufacturer, std::string machine)
{
    m_lexer = std::unique_ptr<lexer>(new lexer());
    init(controller, manufacturer, machine);
}

void parser::init(Controllers controller, Manufacturers manufacturer, std::string machine)
{
    this->controller = controller;
    this->manufacturer = manufacturer;
    this->machine = machine;
    auto topLayer = std::unique_ptr<definition_base>(MachineDefinitions::getDefinition(manufacturer, machine));
    if (topLayer){
        param = topLayer->getParameters();
    }
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
    result.clear();
    start();
    if (result.size()){
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
    if (next == Token::percent)
        match(Token::percent);
    result = list();
}

std::vector<parse_node*> parser::list()
{
    std::vector<parse_node*> blocks;
    while (next != Token::done){
        auto b = block();
        if (b)
            blocks.push_back(b);
    }
    return blocks;
}

parse_node *parser::block()
{
    /*
     * Order defined in standard as N -> G -> dimensions -> I/J/K -> feed function -> spindle speed -> T -> M
     * We parse blocks with the assumption that they follow the most important parts of that definition:
     * N -> G -> parameters -> M
     * The order of parameters are accepted even when they would be wrong according to the standard.
     */
    auto rowItem = new parse_node(Token::block);
    if (next == Token::n_word){
        int blockNum = m_lexer->int_value();
        match(Token::n_word);
        rowItem->appendChild(new parse_node(Token::n_word, blockNum));
    }
    if (next == Token::multi_letter){
        // Such as WHILE-loops
        std::string ml = m_lexer->string_value();
        if (param.functions.block.contains(ml)){
            auto block_definition = param.functions.block.at(ml);
            std::vector<parse_node*> blocks;
            match(Token::multi_letter);
            while (true){
                if (next == Token::multi_letter && block_definition.end_token == m_lexer->string_value()){
                    match(Token::multi_letter);
                    break;
                }
                blocks.push_back(block());
            }
            rowItem->appendChild(new parse_node(Token::block_function, block_definition.type, blocks));
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
            rowItem->appendChild(new parse_node(Token::m_word, active_m_numbers.at(code)));
        }
    }
    if (param.comments.use_parenthesis && next == Token::leftparen
        || param.comments.use_semicolon && next == Token::semicolon){
        rowItem->appendChild(comment());
    }
    if (next == Token::newline){
        match(Token::newline);
    } else if (next != Token::done){
        /*
         * These are pieces of data in the block that shouldn't be there.
         * We will not parse them, but will preserve them as a data node, for debugging purposes.
         * It is also possible that valid data ends up here, if the order of operations is wrong.
         * I.e. M-codes are written before G-codes.
         */
        next = m_lexer->finish_line();
        if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
            errors.push_back(m_lexer->last_error());
            std::cerr << m_lexer->last_error().to_string() << std::endl;
        }
        rowItem->appendChild(new parse_node(Token::left_over_data, m_lexer->string_value()));
    }
    return rowItem;
}

parse_node *parser::expr()
{
    auto p = term();
    return moreterms(p);
}

parse_node *parser::assignment()
{
    auto var = variable();
    if (next == Token::equals){
        match(Token::equals);
        return new parse_node(Token::equals, std::vector<parse_node*>{var, expr()});
    }
    return var;
}

parse_node *parser::variable()
{
    match(next);
    if (next != Token::num_literal)
        return new parse_node(Token::variable, -1);
    int val = m_lexer->int_value();
    match(Token::num_literal);
    return new parse_node(Token::variable, val);
}

parse_node *parser::number()
{
    /*
     * TODO: We need to handle other ways of writing numbers that are valid, i.e. floating point
     *       numbers defined with fixed period position. These will just be parsed as int right now.
     */
    std::string first_part = m_lexer->string_value();
    match(Token::num_literal);
    if (next == Token::period){
        match(Token::period);
        if (next != Token::num_literal)
            return new parse_node(Token::num_int, std::stoi(first_part));
        std::string second_part = m_lexer->string_value();
        match(Token::num_literal);
        double value = stod(first_part + '.' + second_part);
        return new parse_node(Token::num_float, value);
    } else {
        try {
            return new parse_node(Token::num_int, std::stoi(first_part));
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

std::vector<parse_node*> parser::g(bool continuingWord)
{
    int code;
    if (!continuingWord){
        code = m_lexer->int_value();
        match(Token::g_word);
    } else {
        code = last_prepatory_word;
    }
    std::vector<parse_node*> nodes;
    if (active_g_numbers.contains(code)){
        if (code < 5 && !continuingWord){
            last_prepatory_word = code;
        }
        int g_type = active_g_numbers.at(code);
        auto def = param.g[g_type];
        nodes.push_back(new parse_node(Token::g_word, g_type));
        while (def.parameters.contains(next)){
            int param_type;
            if (def.parameter_types.contains(next)){
                param_type = def.parameter_types.at(next);
            } else if (param.parameter_defaults.contains(next)){
                param_type = param.parameter_defaults.at(next);
            } else { // Invalid word
                param_type = next;
            }
            match(next);
            // TODO Handle extended parameters (i.e. X1=100)
            parse_node * e = expr();
            nodes.push_back(new parse_node(Token::parameter, param_type, {e}));
        }
    }
    return nodes;
}

parse_node *parser::comment()
{
    char init_char = next;
    char end_char = ')';
    if (init_char == '(')
        end_char = ')';
    next = m_lexer->finish_comment(end_char);
    if (next == Token::error || next == Token::unknown_function || next == Token::unknown_code){
        errors.push_back(m_lexer->last_error());
        std::cerr << m_lexer->last_error().to_string() << std::endl;
    }
    std::string comment_text = m_lexer->string_value();
    if (next == end_char)
        match(next);
    return new parse_node(Token::comment, comment_text);
}

parse_node *parser::term()
{
    auto *p = factor();
    return morefactors(p);
}

parse_node *parser::moreterms(parse_node *left)
{
    static std::set<int> ops = {Token::plus, Token::minus};
    if (ops.contains(next)){
        int op = next;
        match(op);
        auto p = new parse_node(op, std::vector<parse_node*>{left, term()});
        return moreterms(p);
    } else {
        return left;
    }
}

parse_node *parser::factor()
{
    auto *p = func();
    return morefuncs(p);
}

parse_node *parser::morefactors(parse_node *left)
{
    static std::set<int> ops = {Token::star, Token::slash};
    if (ops.contains(next)){
        int op = next;
        match(op);
        auto p = new parse_node(op, std::vector<parse_node*>{left, factor()});
        return morefactors(p);
    } else {
        return left;
    }
}

parse_node *parser::func()
{
    if (next == Token::num_literal){
        return morefuncs(number());
    } else if (next == Token::leftsquare){
        match(Token::leftsquare);
        auto p = expr();
        match(Token::rightsquare);
        return morefuncs(p);
    } else if (next == Token::multi_letter){
        std::string t = m_lexer->string_value();
        if (param.functions.unary.contains(t)){
            match(Token::multi_letter);
            return morefuncs(new parse_node(Token::unary_function, param.functions.unary[t], {expr()}));
        }
    } else if (param.variables.variable_marker.contains(next)){
        return morefuncs(variable());
    }
    error err {error::Parsing, m_lexer->line(), m_lexer->pos(),
              "Expression non-parseable. Token: " + std::to_string(next) + " Variable: "
                  + m_lexer->string_value()};
    errors.push_back(err);
    std::cerr << err.to_string() << std::endl;
    return nullptr;
}

parse_node *parser::morefuncs(parse_node *left)
{
    if (next == Token::multi_letter){
        std::string t = m_lexer->string_value();
        if (param.functions.binary.contains(t)){
            match(Token::multi_letter);
            return new parse_node(Token::binary_functions, param.functions.binary[t], {left, expr()});
        }
    }
    return left;
}

};
