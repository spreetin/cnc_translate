#ifndef LEXER_H
#define LEXER_H

#include <optional>
#include <string>
#include <set>
#include <variant>
#include "../errors/errors.h"

namespace NCParser {

class lexer
{
public:
    lexer();

    void init(std::string text, std::set<std::string_view> allowed_multiletters = {});
    int next();
    void finish_line();
    void finish_comment(char end = ')');

    [[nodiscard]] int int_value() const;
    [[nodiscard]] double double_value() const;
    [[nodiscard]] std::string string_value() const;
    [[nodiscard]] int line() const{
        return m_line;
    }
    [[nodiscard]] int pos() const{
        return m_pos-1;
    }
    [[nodiscard]] std::string position() const{
        return std::to_string(m_line)+", "+std::to_string(m_pos);
    }
    [[nodiscard]] error last_error() const{
        return m_last_error;
    }

    [[nodiscard]] bool hasValue(){
        return vValue.has_value();
    }

protected:
    [[nodiscard]] virtual int tokenizedReturn(int r) {return r;}; // Just used for unit tests
    [[nodiscard]] char peek(bool acceptWhitespace = false) const;
    [[nodiscard]] std::string grabInt(int offset = 0);
    [[nodiscard]] int getCode(int token);
    [[nodiscard]] std::string checkML(char next);

    std::optional<std::variant<int,double,std::string>> vValue;

    std::string text;
    std::set<std::string_view> multiletter;
    int m_pos = 0;
    int m_line;
    error m_last_error;
    char m_last;
    char m_next = '\0';
};

};

#endif // LEXER_H
