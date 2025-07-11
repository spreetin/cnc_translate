#include "brands/brands.h"
#include "parser/parse_node.h"
#include "brands/parameters.h"
#include <vector>
#include <string_view>

namespace NCParser {

parse_node_p parse(Manufacturers man, std::string_view machine, std::string text);
parse_node_p parse(MachineParameters param, std::string text);
std::string generate(Manufacturers man, std::string_view machine, parse_node_p nodes);
std::string generate(MachineParameters param, parse_node_p nodes);

}
