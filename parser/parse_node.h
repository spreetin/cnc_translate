#ifndef PARSE_NODE_H
#define PARSE_NODE_H

#include <string>
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
        : m_code(code), i_value(value), m_children(children) {value_type = Int;}
    parse_node(int code, double value, std::vector<parse_node_p> children = {})
        : m_code(code), d_value(value), m_children(children) {value_type = Double;}
    parse_node(int code, std::string value, std::vector<parse_node_p> children = {})
        : m_code(code), s_value(value), m_children(children) {value_type = String;}

    // Copy constructors (from naked object or std::shared_ptr)
    parse_node(const parse_node &other);
    parse_node(const parse_node &other, std::vector<parse_node_p> children);
    parse_node(const parse_node_p &other);
    parse_node(const parse_node_p &other, std::vector<parse_node_p> children);

    // ==== CHILDREN ====
    inline void setChildren(std::vector<parse_node_p> v){
        m_children = v;
    }
    inline void prependChild(parse_node_p c){
        m_children.insert(m_children.cbegin(), c);
    }
    inline void appendChild(parse_node_p c){
        m_children.push_back(c);
    }
    inline void appendChildren(std::vector<parse_node_p> c){
        m_children.insert(m_children.end(), c.begin(), c.end());
    }
    [[nodiscard]] inline std::vector<parse_node_p> children() const{
        return m_children;
    }
    [[nodiscard]] inline int childCount() const{
        return m_children.size();
    }
    [[nodiscard]] inline parse_node_p getChild(int index) const{
        if (m_children.size() <= index)
            return nullptr;
        return m_children.at(index);
    }

    // ==== VALUE ====
    void setValue(const int i);
    void setValue(const double d);
    void setValue(const std::string s);

    [[nodiscard]] int intValue();
    [[nodiscard]] double doubleValue();
    [[nodiscard]] std::string stringValue();

    inline const int valueType(){
        return value_type;
    }
    [[nodiscard]] inline bool hasValue() const{
        return (bool)value_type;
    }

    // ==== TYPE ====
    [[nodiscard]] inline int type() const{
        return m_code;
    }
    inline void setType(int type){
        m_code = type;
    }

protected:
    std::string to_string() const;
    void copy_value(const parse_node &other);

    int m_code;
    int i_value;
    double d_value;
    std::string s_value;
    enum {
        None = -1,
        Int = 0,
        Double = 1,
        String = 2
    } value_type;

    std::vector<parse_node_p> m_children;
};

};

#endif // PARSE_NODE_H
