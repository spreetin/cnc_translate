#include <cctype>
#include <string>
#include <set>
#include "lexer.h"
#include "../tokens/tokens.h"

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
        int p = pos;
        if (single){
            return text.at(p);
        }
        while (text.length() > p){
            if (text.at(p) == ' ' || text.at(p) == '\t' || text.at(p) == '\n'){
                p++;
            } else {
                return text.at(p);
            }
        }
        return EOF;
    };

    // Grab the next group of numeric characters
    static auto grabInt = [&]() -> std::string {
        int start = pos-1;
        while (std::isdigit(peek())){
            pos++;
        }
        return text.substr(start, pos-start);
    };

    // Parse what G- or M-code we have
    static auto getCode = [&](int token) -> int {
        if (!std::isdigit(peek())){
            return Token::unknown_code;
        }
        pos++;
        std::string num = grabInt();
        iValue = std::stoi(num);
        return token;
    };

    while (true){
        if (text.length() <= pos){
            return Token::done;
        }
        char next = std::toupper(text.at(pos));
        pos++;
        if (std::isalpha(peek())){
            int start = pos-1;
            while (std::isalpha(peek(true))){
                pos++;
            }
            std::string ml = text.substr(start, pos-start);
            if (multiletter.contains(ml)){
                sValue = ml;
                return Token::multi_letter;
            } else {
                continue;
            }
        } else {
            switch (next){
            case 'G':
                return getCode(Token::g_word);
            case 'M':
                return getCode(Token::m_word);
            case 'N':
            {
                if (!std::isdigit(peek()))
                    continue;
                iValue = std::stoi(grabInt());
                return Token::n_word;
            }
            case 0: case 1: case 2: case 3: case 4:
            case 5: case 6: case 7: case 8: case 9:
            {
                sValue = grabInt();
                iValue = std::stoi(sValue);
                return Token::num_literal;
            }
            case '\n':
                m_line++;
                return Token::newline;
            default:
                return next;
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
