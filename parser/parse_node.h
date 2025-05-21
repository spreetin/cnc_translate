#ifndef PARSE_NODE_H
#define PARSE_NODE_H

#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <memory>

namespace NCParser {

class parse_node;
typedef std::shared_ptr<parse_node> parse_node_p;

class parse_node
{
public:
    // Constructors
    parse_node(int code, std::vector<parse_node_p> children = {})
        : m_code(code), m_children(children) {}
    parse_node(int code, int value, std::vector<parse_node_p> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, double value, std::vector<parse_node_p> children = {})
        : m_code(code), m_value(value), m_children(children) {}
    parse_node(int code, std::string value, std::vector<parse_node_p> children = {})
        : m_code(code), m_value(value), m_children(children) {}

    // Copy constructors (from naked object or std::shared_ptr)
    parse_node(const parse_node &other)
        : m_code(other.m_code), m_value(other.m_value), m_children(other.m_children){}
    parse_node(const parse_node &other, std::vector<parse_node_p> children)
        : m_code(other.m_code), m_value(other.m_value), m_children(children){}
    parse_node(const parse_node_p &other)
        : m_code(other->m_code), m_value(other->m_value), m_children(other->m_children){}
    parse_node(const parse_node_p &other, std::vector<parse_node_p> children)
        : m_code(other->m_code), m_value(other->m_value), m_children(children){}

    // Children
    void setChildren(std::vector<parse_node_p> v){
        m_children = v;
    }
    void prependChild(parse_node_p c){
        m_children.insert(m_children.cbegin(), c);
    }
    void appendChild(parse_node_p c){
        m_children.push_back(c);
    }
    void appendChildren(std::vector<parse_node_p> c){
        m_children.insert(m_children.end(), c.begin(), c.end());
    }
    [[nodiscard]] std::vector<parse_node_p> children() const{
        return m_children;
    }
    [[nodiscard]] int childCount() const{
        return m_children.size();
    }
    [[nodiscard]] parse_node_p getChild(int index) const{
        if (m_children.size() <= index)
            return nullptr;
        return m_children.at(index);
    }

    // Values
    void setValue(std::variant<int,double,std::string> v){
        m_value = v;
    }

    [[nodiscard]] int intValue();
    [[nodiscard]] double doubleValue();
    [[nodiscard]] std::string stringValue();

    constexpr int valueType(){
        return m_value->index();
    }
    [[nodiscard]] bool hasValue() const{
        return m_value.has_value();
    }

    // Type
    [[nodiscard]] int type() const{
        return m_code;
    }
    void setType(int type){
        m_code = type;
    }

protected:
    std::string to_string() const;

    int m_code;
    std::optional<std::variant<int,double,std::string>> m_value;
    std::vector<parse_node_p> m_children;
};

};

#endif // PARSE_NODE_H
