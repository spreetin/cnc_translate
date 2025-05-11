#ifndef PARSE_NODE_GEN_H
#define PARSE_NODE_GEN_H

#include <string>
#include <memory>

namespace NCParser {

class parse_node;
struct MachineParameters;

class parse_node_gen
{
public:
    parse_node_gen() {}

    static std::string generate(std::shared_ptr<parse_node>block, MachineParameters *param);

protected:
    static std::string expr(std::shared_ptr<parse_node>n, MachineParameters *param);
};

class pnw
{
    pnw(std::shared_ptr<parse_node>n, MachineParameters *param) : n(n), param(param) {};

    friend std::ostream& operator<<(std::ostream& os, const pnw &pnw);

protected:
    std::string to_string() const;
    std::shared_ptr<parse_node>n;
    MachineParameters *param;
};

}

#endif // PARSE_NODE_GEN_H
