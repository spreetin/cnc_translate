#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <set>
#include <variant>
#include "../errors/errors.h"

namespace NCParser {

class lexer
{
public:
    lexer();

    void init(std::string text, std::set<std::string> allowed_multiletters = {});
    int next();
    int finish_line();
    int finish_comment(char end = ')');

    int int_value();
    double double_value();
    std::string string_value();
    std::variant<int,double,std::string> value(){
        return vValue;
    }
    int line(){
        return m_line;
    }
    int pos(){
        return m_pos-1;
    }
    std::string position(){
        return std::to_string(m_line)+", "+std::to_string(m_pos);
    }
    error last_error(){
        return m_last_error;
    }

protected:
    virtual int tokenizedReturn(int r) {return r;}; // Just used for unit tests
    char peek(bool single = false);
    std::string grabInt(int offset = 0);
    int getCode(int token);

    std::variant<int,double,std::string> vValue;

    std::string text;
    std::set<std::string> multiletter;
    int m_pos = 0;
    int m_line;
    error m_last_error;
};

};

#endif // LEXER_H
