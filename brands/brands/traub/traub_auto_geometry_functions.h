#ifndef TRAUB_AUTO_GEOMETRY_FUNCTIONS_H
#define TRAUB_AUTO_GEOMETRY_FUNCTIONS_H

#include "../../auto_geometry_function.h"

namespace NCParser {

class traub_function_g101 : public auto_geometry_function
{
public:
    traub_function_g101() : auto_geometry_function() {}

    std::vector<std::shared_ptr<parse_node>> process(std::map<char,std::variant<int,double,std::string>> parameters);
};

class traub_function_g102 : public auto_geometry_function
{
public:
    traub_function_g102() : auto_geometry_function() {}

    std::vector<std::shared_ptr<parse_node>> process(std::map<char,std::variant<int,double,std::string>> parameters);
};

class traub_function_g105 : public auto_geometry_function
{
public:
    traub_function_g105() : auto_geometry_function() {}

    std::vector<std::shared_ptr<parse_node>> process(std::map<char,std::variant<int,double,std::string>> parameters);
};

class traub_function_g106 : public auto_geometry_function
{
public:
    traub_function_g106() : auto_geometry_function() {}

    std::vector<std::shared_ptr<parse_node>> process(std::map<char,std::variant<int,double,std::string>> parameters);
};

};

#endif // TRAUB_AUTO_GEOMETRY_FUNCTIONS_H
