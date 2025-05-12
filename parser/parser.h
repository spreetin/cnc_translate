#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <variant>
#include <memory>
#include <set>
#include "../lexer/lexer.h"
#include "parse_node.h"
#include "../brands/parameters.h"
#include "../brands/brands.h"
#include "../errors/errors.h"

namespace NCParser {

class parser
{
public:
    parser(Manufacturers manufacturer, std::string machine);

    bool parse(std::string text);

    std::vector<parse_node_p> result(){
        return m_result;
    }

protected:
    void init(Manufacturers manufacturer, std::string machine);

    void match(int code);

    void start();
    std::vector<parse_node_p> list();
    parse_node_p block();
    parse_node_p expr();
    parse_node_p assignment();
    parse_node_p variable();
    parse_node_p number();
    std::vector<parse_node_p> g(bool continuingWord = false);
    parse_node_p grabParameterData(std::vector<int> sub_types);
    parse_node_p comment();

    // Math expression parsing
    parse_node_p term();
    parse_node_p moreterms(parse_node_p left);
    parse_node_p factor();
    parse_node_p morefactors(parse_node_p left);
    parse_node_p func();
    parse_node_p morefuncs(parse_node_p left);

    std::map<int,std::variant<int,double>> values;
    std::set<std::string> allowed_multiletter;
    std::map<int,int> active_g_numbers;
    std::map<int,int> active_m_numbers;

    std::unique_ptr<lexer> m_lexer;
    Manufacturers manufacturer;
    std::string machine;
    MachineParameters param;

    int next;
    std::map<int,std::variant<int,double>> variables;
    int last_prepatory_word = -1;
    std::vector<error> errors;

    std::vector<parse_node_p> m_result;
};

};

#endif // PARSER_H
