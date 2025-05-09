#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <set>

namespace NCParser {

class lexer
{
public:
    lexer();

    void init(std::string text, std::set<std::string> allowed_multiletters);
    int next();
    int finishLine();
    int finishComment(char end = ')');

    int intValue(){
        return iValue;
    }
    double doubleValue(){
        return fValue;
    }
    std::string stringValue(){
        return sValue;
    }
    int line(){
        return m_line;
    }

protected:
    virtual int tokenizedReturn(int r) {return r;}; // Just used for unit tests

    int iValue = -1;
    double fValue = -1;
    std::string sValue;

    std::string text;
    std::set<std::string> multiletter;
    int pos;
    int m_line;
};

};

#endif // LEXER_H
