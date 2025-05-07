#ifndef PARSE_NODE_H
#define PARSE_NODE_H

#include <string>
#include <variant>
#include <vector>
#include <limits>

namespace NCParser {

class parse_node
{
public:
    // Constructors
    parse_node(int code, std::vector<parse_node*> children = {})
        : m_code(code), m_children(children) {}
    parse_node(int code, int value, std::vector<parse_node*> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, double value, std::vector<parse_node*> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, std::string value, std::vector<parse_node*> children = {})
        : m_code(code), m_value(value), m_children(children) {}

    ~parse_node();

    // Children
    void setChildren(std::vector<parse_node*> v){
        m_children = v;
    }
    void prependChild(parse_node *c){
        m_children.insert(m_children.cbegin(), c);
    }
    void appendChild(parse_node *c){
        m_children.push_back(c);
    }

    // Values
    void setValue(std::variant<int,double,std::string> v){
        m_value = v;
    }
    std::variant<int,double,std::string> value(){
        return m_value;
    }
    int intValue(){
        return std::get<int>(m_value);
    }
    double doubleValue(){
        return std::get<double>(m_value);
    }
    std::string stringValue(){
        return std::get<std::string>(m_value);
    }

protected:
    int m_code;
    std::variant<int,double,std::string> m_value;
    std::vector<parse_node*> m_children;
};

};

#endif // PARSE_NODE_H
