#include "parser.h"
#include "../tokens/iso6983.h"
#include "../brands/machine_definitions.h"
#include <iostream>

namespace NCParser {

parser::parser()
    : gcode_parser(this), mcode_parser(this)
{
    m_lexer = std::unique_ptr<lexer>(new lexer());
}

void parser::init(Controllers controller, Manufacturers manufacturer, std::string machine)
{
    this->controller = controller;
    this->manufacturer = manufacturer;
    this->machine = machine;
    gCodes = ISO6983::gCodes();
    mCodes = ISO6983::mCodes();
    fixedCycleDefinition = ISO6983::fixedCycleDefinition();
    auto topLayer = std::unique_ptr<definition_base>(MachineDefinitions::getDefinition(manufacturer, machine));
    if (topLayer){
        topLayer->updateData(gCodes, mCodes, fixedCycleDefinition);
        param = topLayer->getParameters();
    }
}

bool parser::parse(std::string text)
{
    m_lexer->init(text, param);
    result.clear();
    start();
    if (result.size()){
        return true;
    } else {
        return false;
    }
}

void parser::clear()
{
    values.clear();
    gCodes->clear();
    mCodes->clear();
    fixedCycleDefinition->clear();
}

void parser::match(int code)
{
    if (next != code){
        std::cout << "Faulty match: " << code << " on line " << m_lexer->line() << std::endl;
    }
    if (next != Token::done)
        next = m_lexer->next();
}

void parser::start()
{
    next = m_lexer->next();
    if (next == Token::percent)
        match(Token::percent);
    result = list();
}

std::vector<parse_node*> parser::list()
{
    std::vector<parse_node*> blocks;
    while (next != Token::done){
        blocks.push_back(block());
    }
    return blocks;
}

parse_node *parser::block()
{
    auto rowItem = new parse_node(Token::block);
    while (next != Token::newline){
        if (next == Token::n_word){

        } else if (next == Token::g_word){
            int code = m_lexer->intValue();
            match(Token::g_word);
            rowItem->appendChild(gcode_parser.parse(code));
        } else if (next == Token::m_word){
            int code = m_lexer->intValue();
            match(Token::g_word);
            rowItem->appendChild(mcode_parser.parse(code));
        } else if (next == Token::variable){
            int id = m_lexer->intValue();
            variables.insert(id, {});
            match(Token::variable);
            rowItem->appendChild(new parse_node(Token::variable, id));
        } else if (next == Token::comment){
            std::string text = m_lexer->stringValue();
            match(Token::comment);
            rowItem->appendChild(new parse_node(Token::comment, text));
        }
    }
    return rowItem;
}

std::vector<parse_node*> parser::optionlist(std::set<int> allowed)
{
    std::vector<parse_node*> nodes;
    while (allowed.contains(next)){
        int op = next;
        match(op);
        if (next == Token::num_float){
            nodes.push_back(new parse_node(op, m_lexer->doubleValue()));
            match(Token::num_float);
        } else if (next == Token::num_int){
            nodes.push_back(new parse_node(op, m_lexer->intValue()));
            match(Token::num_int);
        } else {
            break;
        }
    }
    return nodes;
}

};
