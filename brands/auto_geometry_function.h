#ifndef AUTO_GEOMETRY_FUNCTION_H
#define AUTO_GEOMETRY_FUNCTION_H

#include <map>
#include <variant>
#include <string>
#include <vector>
#include "../parser/parse_node.h"

namespace NCParser {

class auto_geometry_function
{
public:
    auto_geometry_function();

    virtual std::vector<parse_node_p> process(std::map<char,std::variant<int,double,std::string>> parameters) = 0;
};

};

#endif // AUTO_GEOMETRY_FUNCTION_H
