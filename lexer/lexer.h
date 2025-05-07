#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <set>
#include "../brands/brands.h"

namespace NCParser {

class lexer
{
public:
    lexer();

    void init(std::string text, MachineParameters param);
    int next();

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

    static const std::set<int> g_codes;
    static const std::set<int> m_codes;

protected:
    int iValue = -1;
    double fValue = -1;
    std::string sValue;

    std::string text;
    int pos;
    int m_line;

    MachineParameters param;
};

};

#endif // LEXER_H
