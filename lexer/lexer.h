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

    int int_value() const;
    double double_value() const;
    std::string string_value() const;
    int line() const{
        return m_line;
    }
    int pos() const{
        return m_pos-1;
    }
    std::string position() const{
        return std::to_string(m_line)+", "+std::to_string(m_pos);
    }
    error last_error() const{
        return m_last_error;
    }

protected:
    virtual int tokenizedReturn(int r) {return r;}; // Just used for unit tests
    char peek(bool single = false) const;
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
