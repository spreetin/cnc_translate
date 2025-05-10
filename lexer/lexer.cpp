#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <set>
#include <algorithm>
#include "lexer.h"
#include "../tokens/tokens.h"

#ifdef TESTING
#define RETURN(x) tokenizedReturn(x)
#else
#define RETURN(x) x
#endif


namespace NCParser {

lexer::lexer() {}

void lexer::init(std::string text, std::set<std::string> allowed_multiletters)
{
    this->text = text;
    multiletter = allowed_multiletters;
    m_pos = 0;
    m_line = 0;
}

int lexer::next()
{
    while (true){
        if (text.length() <= m_pos){
            return RETURN(Token::done);
        }
        char next = std::toupper(text.at(m_pos));
        m_pos++;
        if (std::isalpha(next) && m_pos < text.size() && std::isalpha(text.at(m_pos))){
            int start = m_pos-1;
            while (m_pos < text.size() && std::isalpha(text.at(m_pos))){
                m_pos++;
            }
            std::string ml = text.substr(start, m_pos-start);
            vValue = ml;
            if (multiletter.contains(ml)){
                return RETURN(Token::multi_letter);
            } else {
                m_last_error = {error::Lexing, m_line, pos(), "Function not defined: \"" + ml +"\""};
                return RETURN(Token::unknown_function);
            }
        } else {
            switch (next){
            case ' ':
            case '\t':
            case '\r':
                continue;
            case 'G':
                return RETURN(getCode(Token::g_word));
            case 'M':
                return RETURN(getCode(Token::m_word));
            case 'N':
            {
                if (!std::isdigit(peek()))
                    continue;
                int val = std::stoi(grabInt());
                vValue = val;
                return RETURN(Token::n_word);
            }
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                vValue = grabInt(-1);
                return RETURN(Token::num_literal);
            }
            case '\n':
                m_line++;
                return RETURN(Token::newline);
            default:
                return RETURN(next);
            }
        }
    }
    m_last_error = {error::Lexing, m_line, pos(), "Undefined token encountered on \""
                                                      + text.substr(pos(), text.find('\n', m_pos))};
    return Token::error;
}

int lexer::finish_line()
{
    int start = m_pos;
    while (m_pos < text.size() && text.at(m_pos) != '\n'){
        m_pos++;
    }
    vValue = text.substr(start, m_pos-start);
    return next();
}

int lexer::finish_comment(char end)
{
    int start = m_pos;
    while (m_pos < text.size() && text.at(m_pos) != '\n' && text.at(m_pos) != end){
        m_pos++;
    }
    vValue = text.substr(start, m_pos-start);
    return next();
}

int lexer::int_value()
{
    switch (vValue.index()){
    case 0:
        return std::get<int>(vValue);
    case 1:
        return std::floor(std::get<double>(vValue));
    case 2:
        return std::stoi(std::get<std::string>(vValue));
    }
    return -1;
}

double lexer::double_value()
{
    switch (vValue.index()){
    case 0:
        return std::get<int>(vValue);
    case 1:
        return std::get<double>(vValue);
    case 2:
        return std::stod(std::get<std::string>(vValue));
    }
    return -1;
}

std::string lexer::string_value()
{
    switch (vValue.index()){
    case 0:
        return std::to_string(std::get<int>(vValue));
    case 1:
        return std::to_string(std::get<double>(vValue));
    case 2:
        return std::get<std::string>(vValue);
    }
    return "";
}

char lexer::peek(bool single)
{
    if (single){
        if (text.length() > m_pos){
            return text.at(m_pos);
        } else {
            return '\0';
        }
    }
    int p = m_pos;
    while (text.length() > p){
        if (text.at(p) == ' ' || text.at(p) == '\t'){
            p++;
        } else {
            return text.at(p);
        }
    }
    return EOF;
}

std::string lexer::grabInt(int offset)
{
    int start = m_pos+offset;
    while (m_pos < text.size() && std::isdigit(peek())){
        m_pos++;
    }
    std::string r = text.substr(start, m_pos-start);
    r.erase(std::remove_if(r.begin(), r.end(), [](const char &c){
                return c==' ' || c=='\t';}), r.end());
    return r;
}

int lexer::getCode(int token)
{
    std::string num;
    try {
        if (!std::isdigit(peek())){
            m_last_error = {error::Lexing, m_line, pos(), "Can't extract code name on \""
                                                              + text.substr(pos(), text.find('\n', m_pos))};
            return Token::unknown_code;
        }
        num = grabInt();
        //std::cout << num;
        vValue = std::stoi(num);
        return token;
    } catch (std::invalid_argument) {
        vValue = num;
        m_last_error = {error::Lexing, m_line, pos(), "Code name illegal on \""
                                                          + text.substr(pos(), text.find('\n', m_pos))};
        return Token::unknown_code;
    }
}

};
