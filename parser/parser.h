#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <variant>
#include <memory>
#include <set>
#include "../tokens/tokens.h"
#include "../lexer/lexer.h"
#include "parse_gcodes.h"
#include "parse_mcodes.h"
#include "parse_node.h"
#include "../brands/brands.h"

namespace NCParser {

class parser
{
public:
    parser();

    void init(Controllers controller, Manufacturers manufacturer, std::string machine);
    bool parse(std::string text);

    void clear();

    friend class parse_gcodes;
    friend class parse_mcodes;

private:
    void match(int code);

    void start();
    std::vector<parse_node *> list();
    parse_node *block();

    std::vector<parse_node*> optionlist(std::set<int> allowed = std::set<int>(
                                           {Token::x_word, Token::y_word, Token::z_word, Token::u_word,
                                            Token::v_word, Token::w_word, Token::p_word, Token::q_word,
                                            Token::r_word, Token::a_word, Token::b_word}));

    std::map<int,std::variant<int,double>> values;
    std::shared_ptr<std::map<int,GCode>> gCodes;
    std::shared_ptr<std::map<int,MCode>> mCodes;
    std::shared_ptr<std::map<int,FixedCycleDefinition>> fixedCycleDefinition;

    std::unique_ptr<lexer> m_lexer;
    Controllers controller;
    Manufacturers manufacturer;
    std::string machine;
    MachineParameters param;

    int next;
    std::map<int,std::variant<int,double>> variables;

    parse_gcodes gcode_parser;
    parse_mcodes mcode_parser;

    std::vector<parse_node*> result;
};

};

#endif // PARSER_H
