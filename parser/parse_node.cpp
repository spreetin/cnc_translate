#include <limits>
#include "parse_node.h"
#include "../generator/parse_node_gen.h"

namespace NCParser {

int parse_node::intValue(){
    if (!m_value.has_value())
        return std::numeric_limits<int>::min();
    try {
        return std::get<int>(m_value.value());
    } catch (std::bad_variant_access){
        return std::numeric_limits<int>::min();
    }
}

double parse_node::doubleValue(){
    if (!m_value.has_value())
        return std::numeric_limits<int>::min();
    try {
        switch (m_value->index()){
        case 0: return (double) std::get<0>(m_value.value());
        case 1: return std::get<double>(m_value.value());
        default: return std::stod(std::get<2>(m_value.value()));
        }
    } catch (std::invalid_argument){
        return std::numeric_limits<double>::min();
    }
}

std::string parse_node::stringValue(){
    if (!m_value.has_value())
        return "";
    try {
        return std::get<std::string>(m_value.value());
    } catch (std::bad_variant_access e){
        switch (m_value->index()){
        case 0:
            return std::to_string(std::get<int>(m_value.value()));
        case 1:
            return std::to_string(std::get<double>(m_value.value()));
        default:
            return "";
        }
    }
}

};
