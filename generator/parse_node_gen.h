#ifndef PARSE_NODE_GEN_H
#define PARSE_NODE_GEN_H

#include <string>
#include <memory>

namespace NCParser {

class parse_node;
typedef std::shared_ptr<parse_node> parse_node_p;
struct MachineParameters;

class parse_node_gen
{
public:
    parse_node_gen() {}

    static std::string generate(parse_node_p block, MachineParameters *param);

protected:
    static std::string expr(parse_node_p n, MachineParameters *param);
};

class pnw
{
    pnw(parse_node_p n, MachineParameters *param) : n(n), param(param) {};

    friend std::ostream& operator<<(std::ostream& os, const pnw &pnw);

protected:
    std::string to_string() const;
    parse_node_p n;
    MachineParameters *param;
};

}

#endif // PARSE_NODE_GEN_H
