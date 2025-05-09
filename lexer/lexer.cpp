#include <cctype>
#include <iostream>
#include <string>
#include <set>
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
    pos = 0;
    m_line = 0;
}

int lexer::next()
{
    // Helper to peek forward
    static auto peek = [this](bool single = false) -> char {
        if (single){
            return text.at(pos);
        }
        int p = pos;
        while (text.length() > p){
            if (text.at(p) == ' ' || text.at(p) == '\t'){
                p++;
            } else {
                return text.at(p);
            }
        }
        return EOF;
    };

    // Grab the next group of numeric characters
    static auto grabInt = [&](int offset = 0) -> std::string {
        while (pos < text.size() && (text.at(pos) == ' ' || text.at(pos) == '\t'))
            pos++;
        int start = pos+offset;
        while (pos < text.size() && std::isdigit(text.at(pos))){
            pos++;
        }
        return text.substr(start, pos-start);
    };

    // Parse what G- or M-code we have
    static auto getCode = [&](int token) -> int {
        if (!std::isdigit(peek())){
            return Token::unknown_code;
        }
        std::string num = grabInt();
        //std::cout << num;
        iValue = std::stoi(num);
        return token;
    };

    while (true){
        if (text.length() <= pos){
            return RETURN(Token::done);
        }
        char next = std::toupper(text.at(pos));
        pos++;
        if (std::isalpha(next) && pos < text.size() && std::isalpha(text.at(pos))){
            int start = pos-1;
            while (pos < text.size() && std::isalpha(text.at(pos))){
                pos++;
            }
            std::string ml = text.substr(start, pos-start);
            sValue = ml;
            if (multiletter.contains(ml)){
                return RETURN(Token::multi_letter);
            } else {
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
                iValue = std::stoi(grabInt());
                return RETURN(Token::n_word);
            }
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                sValue = grabInt(-1);
                iValue = std::stoi(sValue);
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
    return Token::error;
}

int lexer::finishLine()
{
    int start = pos;
    while (text.at(pos) != '\n'){
        pos++;
    }
    sValue = text.substr(start, pos-start);
    return Token::newline;
}

int lexer::finishComment(char end)
{
    int start = pos;
    while (text.at(pos) != '\n' && text.at(pos) != end){
        pos++;
    }
    sValue = text.substr(start, pos-start);
    int result = text.at(pos);
    pos++;
    return result;
}

};
