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

    std::vector<parse_node*> result(){
        return m_result;
    }

protected:
    void init(Manufacturers manufacturer, std::string machine);

    void match(int code);

    void start();
    std::vector<parse_node *> list();
    parse_node *block();
    parse_node *expr();
    parse_node *assignment();
    parse_node *variable();
    parse_node *number();
    std::vector<parse_node *> g(bool continuingWord = false);
    parse_node *grabParameterData(std::vector<int> sub_types);
    parse_node *comment();

    // Math expression parsing
    parse_node *term();
    parse_node *moreterms(parse_node *left);
    parse_node *factor();
    parse_node *morefactors(parse_node *left);
    parse_node *func();
    parse_node *morefuncs(parse_node *left);

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

    std::vector<parse_node*> m_result;
};

};

#endif // PARSER_H
