#include <gtest/gtest.h>
#include "../generator/parse_node_gen.h"
#include "../parser/parse_node.h"
#include "../tokens/tokens.h"
#include "../brands/brands.h"
#include "../generator/generator.h"

using namespace NCParser;

class ParseNodeGen : public generator, public testing::Test{
public:
    ParseNodeGen() : generator(Traub, "TX8H"), testing::Test() {}

protected:
};

TEST_F(ParseNodeGen, number){
    parse_node * n = new parse_node(Token::num_int, 123);
    EXPECT_EQ(parse_node_gen::generate(n, &param), "123");
}
