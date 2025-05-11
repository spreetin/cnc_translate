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

    std::vector<std::shared_ptr<parse_node>> result(){
        return m_result;
    }

protected:
    void init(Manufacturers manufacturer, std::string machine);

    void match(int code);

    void start();
    std::vector<std::shared_ptr<parse_node>> list();
    std::shared_ptr<parse_node>block();
    std::shared_ptr<parse_node>expr();
    std::shared_ptr<parse_node>assignment();
    std::shared_ptr<parse_node>variable();
    std::shared_ptr<parse_node>number();
    std::vector<std::shared_ptr<parse_node>> g(bool continuingWord = false);
    std::shared_ptr<parse_node>grabParameterData(std::vector<int> sub_types);
    std::shared_ptr<parse_node>comment();

    // Math expression parsing
    std::shared_ptr<parse_node>term();
    std::shared_ptr<parse_node>moreterms(std::shared_ptr<parse_node>left);
    std::shared_ptr<parse_node>factor();
    std::shared_ptr<parse_node>morefactors(std::shared_ptr<parse_node>left);
    std::shared_ptr<parse_node>func();
    std::shared_ptr<parse_node>morefuncs(std::shared_ptr<parse_node>left);

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

    std::vector<std::shared_ptr<parse_node>> m_result;
};

};

#endif // PARSER_H
