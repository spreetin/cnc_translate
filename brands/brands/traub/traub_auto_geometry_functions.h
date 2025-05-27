#ifndef TRAUB_AUTO_GEOMETRY_FUNCTIONS_H
#define TRAUB_AUTO_GEOMETRY_FUNCTIONS_H

#include <map>
#include <string>
#include <variant>
#include <vector>
#include "../../../parser/parse_node.h"

namespace NCParser {

std::vector<parse_node_p> traub_function_g101(std::map<char,std::variant<int,double,std::string>> parameters);
std::vector<parse_node_p> traub_function_g102(std::map<char,std::variant<int,double,std::string>> parameters);
std::vector<parse_node_p> traub_function_g105(std::map<char,std::variant<int,double,std::string>> parameters);
std::vector<parse_node_p> traub_function_g106(std::map<char,std::variant<int,double,std::string>> parameters);

};

#endif // TRAUB_AUTO_GEOMETRY_FUNCTIONS_H
