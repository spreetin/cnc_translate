#ifndef PARSE_NODE_H
#define PARSE_NODE_H

#include <string>
#include <variant>
#include <vector>
#include <memory>

namespace NCParser {

class parse_node
{
public:
    // Constructors
    parse_node(int code, std::vector<std::shared_ptr<parse_node>> children = {})
        : m_code(code), m_children(children) {}
    parse_node(int code, int value, std::vector<std::shared_ptr<parse_node>> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, double value, std::vector<std::shared_ptr<parse_node>> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, std::string value, std::vector<std::shared_ptr<parse_node>> children = {})
        : m_code(code), m_value(value), m_children(children) {}

    // Children
    void setChildren(std::vector<std::shared_ptr<parse_node>> v){
        m_children = v;
    }
    void prependChild(std::shared_ptr<parse_node>c){
        m_children.insert(m_children.cbegin(), c);
    }
    void appendChild(std::shared_ptr<parse_node>c){
        m_children.push_back(c);
    }
    void appendChildren(std::vector<std::shared_ptr<parse_node>> c){
        m_children.insert(m_children.end(), c.begin(), c.end());
    }
    std::vector<std::shared_ptr<parse_node>> children(){
        return m_children;
    }
    int childCount(){
        return m_children.size();
    }
    std::shared_ptr<parse_node>getChild(int index){
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
    std::vector<std::shared_ptr<parse_node>> m_children;
};

};

#endif // PARSE_NODE_H
