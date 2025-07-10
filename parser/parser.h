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
#include "../tokens/tokens.h"

namespace NCParser {

class parser
{
public:
    parser(Manufacturers manufacturer, std::string_view machine);
    parser(MachineParameters params);

    bool parse(std::string text);

    [[nodiscard]] parse_node_p result(){
        return std::make_shared<parse_node>(Token::list, m_result);
    }

    [[nodiscard]] MachineParameters & getParam(){
        return param;
    }

    [[nodiscard]] std::set<std::string_view> getMultiletter(){
        return allowed_multiletter;
    }

protected:
    void init(Manufacturers manufacturer, std::string_view machine);
    void init(MachineParameters params);

    void match(int code);

    void start();
    [[nodiscard]] std::vector<parse_node_p> list();
    [[nodiscard]] parse_node_p block();
    [[nodiscard]] parse_node_p expr();
    [[nodiscard]] parse_node_p assignment();
    [[nodiscard]] parse_node_p variable();
    [[nodiscard]] parse_node_p number();
    [[nodiscard]] std::vector<parse_node_p> g(bool continuingWord = false);
    [[nodiscard]] parse_node_p grabParameterData(std::vector<int> sub_types);
    [[nodiscard]] parse_node_p comment();

    // Math expression parsing
    [[nodiscard]] parse_node_p term();
    [[nodiscard]] parse_node_p moreterms(parse_node_p left);
    [[nodiscard]] parse_node_p factor();
    [[nodiscard]] parse_node_p morefactors(parse_node_p left);
    [[nodiscard]] parse_node_p func();
    [[nodiscard]] parse_node_p morefuncs(parse_node_p left);

    std::map<int,std::variant<int,double>> values;
    std::set<std::string_view> allowed_multiletter;
    std::map<int,int> active_g_numbers;
    std::map<int,int> active_m_numbers;

    std::unique_ptr<lexer> m_lexer;
    Manufacturers manufacturer;
    std::string_view machine;
    MachineParameters param;

    int next;
    std::map<int,std::variant<int,double>> variables;
    int last_prepatory_word = -1;
    std::vector<error> errors;

    std::vector<parse_node_p> m_result;
};

};

#endif // PARSER_H
