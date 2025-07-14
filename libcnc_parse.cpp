#include "generator/generator.h"
#include "parser/parser.h"
#include <string_view>

namespace NCParser {

parse_node_p parse(Manufacturers man, std::string_view machine,
                   std::string text) {
  parser p = parser(man, machine);
  p.parse(text);
  auto r = p.result();
  return r;
}

parse_node_p parse(MachineParameters param, std::string text) {
  parser p = parser(param);
  p.parse(text);
  auto r = p.result();
  return r;
}

std::string generate(Manufacturers man, std::string_view machine,
                     parse_node_p nodes) {
  generator g = generator(man, machine);
  auto r = g.generate(nodes);
  return r;
}

std::string generate(MachineParameters param, parse_node_p nodes) {
  generator g = generator(param);
  auto r = g.generate(nodes);
  return r;
}

std::string printParseTree(MachineParameters param, parse_node_p rootNode) {
  generator g = generator(param);
  return g.printTree(rootNode);
}

} // namespace NCParser
