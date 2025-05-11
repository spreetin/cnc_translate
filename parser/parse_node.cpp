#include <limits>
#include "parse_node.h"
#include "../generator/parse_node_gen.h"

namespace NCParser {

int parse_node::intValue(){
    try {
        return std::get<int>(m_value);
    } catch (std::bad_variant_access){
        return std::numeric_limits<int>::min();
    }
}

double parse_node::doubleValue(){
    try {
        return std::get<double>(m_value);
    } catch (std::bad_variant_access){
        return std::numeric_limits<double>::min();
    }
}

std::string parse_node::stringValue(){
    try {
        return std::get<std::string>(m_value);
    } catch (std::bad_variant_access e){
        return std::string();
    }
}

};
