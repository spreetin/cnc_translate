#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include <sstream>

namespace NCParser {

struct error {
    enum {
        Lexing,
        Parsing,
        Generating,
    } type;
    int line = -1;
    int pos = -1;
    std::string msg;

    std::string to_string(){
        std::stringstream ss;
        switch (type){
        case Lexing:
            ss << "Lexing error"; break;
        case Parsing:
            ss << "Parsing error"; break;
        case Generating:
            ss << "Code generation error"; break;
        }
        if (line != -1){
            if (pos != -1){
                ss << " on position "  << std::to_string(line) << ":" << std::to_string(pos) << ".";
            } else {
                ss << " on line " << std::to_string(line) << ".";
            }
        }
        ss << " Msg: " << msg << "\n";
        return ss.str();
    }
};

};

#endif // ERRORS_H
