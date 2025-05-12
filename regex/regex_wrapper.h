#ifndef REGEX_WRAPPER_H
#define REGEX_WRAPPER_H

#include <string>
#include "../brands/parameter_definitions.h"

namespace NCParser {

class regex_wrapper
{
public:
    regex_wrapper();

    static bool matchVariable(RegexToken token, std::string text);
};

}

#endif // REGEX_WRAPPER_H
