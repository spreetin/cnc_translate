#include <limits>
#include <cmath>
#include <cstdlib>
#include "parse_node.h"
#include "../generator/parse_node_gen.h"

namespace NCParser {

parse_node::parse_node(const parse_node &other)
    : m_code(other.m_code), m_children(other.m_children){
    copy_value(other);
}

parse_node::parse_node(const parse_node &other, std::vector<parse_node_p> children)
    : m_code(other.m_code), m_children(children){
    copy_value(other);
}

parse_node::parse_node(const parse_node_p &other)
    : m_code(other->m_code), m_children(other->m_children){
    copy_value(*other.get());
}

parse_node::parse_node(const parse_node_p &other, std::vector<parse_node_p> children)
    : m_code(other->m_code), m_children(children){
    copy_value(*other.get());
}

void parse_node::setValue(const int i)
{
    i_value = i;
    value_type = Int;
}

void parse_node::setValue(const double d)
{
    d_value = d;
    value_type = Double;
}

void parse_node::setValue(const std::string s)
{
    s_value = s;
    value_type = String;
}

int parse_node::intValue(){
    switch (value_type){
    case None:
        return std::numeric_limits<int>::min();
    case Int:
        return i_value;
    case Double:
        return d_value;
    case String:
        return strtol(s_value.c_str(), nullptr, 0);
    }
}

double parse_node::doubleValue(){
    switch (value_type){
    case None:
        return std::numeric_limits<double>::min();
    case Int:
        return i_value;
    case Double:
        return d_value;
    case String:
        return strtod(s_value.c_str(), nullptr);
    }
}

std::string parse_node::stringValue(){
    switch (value_type){
    case None:
        return "";
    case Int:
        return std::to_string(i_value);
    case Double:
        return std::to_string(d_value);
    case String:
        return s_value;
    }
}

void parse_node::copy_value(const parse_node &other)
{
    switch (other.value_type){
    case None:
        break;
    case Int:
        i_value = other.i_value; break;
    case Double:
        d_value = other.d_value; break;
    case String:
        s_value = other.s_value; break;
        break;
    }
    value_type = other.value_type;
}

}
