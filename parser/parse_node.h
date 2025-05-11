#ifndef PARSE_NODE_H
#define PARSE_NODE_H

#include <string>
#include <variant>
#include <vector>

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
    void appendChildren(std::vector<parse_node*> c){
        m_children.insert(m_children.end(), c.begin(), c.end());
    }
    std::vector<parse_node*> children(){
        return m_children;
    }
    int childCount(){
        return m_children.size();
    }
    parse_node *getChild(int index){
        if (m_children.size() <= index)
            return nullptr;
        return m_children.at(index);
    }

    // Values
    void setValue(std::variant<int,double,std::string> v){
        m_value = v;
    }
    std::variant<int,double,std::string> value(){
        return m_value;
    }
    int intValue();
    double doubleValue();
    std::string stringValue();

    // Type
    int type(){
        return m_code;
    }

protected:
    std::string to_string() const;

    int m_code;
    std::variant<int,double,std::string> m_value;
    std::vector<parse_node*> m_children;
};

};

#endif // PARSE_NODE_H
