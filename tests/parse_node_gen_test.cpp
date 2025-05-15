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
    auto n = parse_node_p{new parse_node(Token::num_int, 123)};
    EXPECT_EQ(generate(n), "123");
}

TEST_F(ParseNodeGen, variable){
    auto n = parse_node_p{new parse_node(Token::variable, 501)};
    EXPECT_EQ(generate(n), "L501");
}

TEST_F(ParseNodeGen, equals){
    auto n = parse_node_p{new parse_node(Token::equals, -1, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "100 = 100");
}

TEST_F(ParseNodeGen, star){
    auto n = parse_node_p{new parse_node(Token::star, -1, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "[100 * 100]");
}

TEST_F(ParseNodeGen, plus){
    auto n = parse_node_p{new parse_node(Token::plus, -1, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "[100 + 100]");
}

TEST_F(ParseNodeGen, minus){
    auto n = parse_node_p{new parse_node(Token::minus, -1, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "[100 - 100]");
}

TEST_F(ParseNodeGen, slash){
    auto n = parse_node_p{new parse_node(Token::slash, -1, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "[100 / 100]");
}

TEST_F(ParseNodeGen, binary_functions){
    auto n = parse_node_p{new parse_node(Token::binary_functions, f_comparison_greater, {
        parse_node_p{new parse_node(Token::num_literal, 100)},
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "[100 GT 100]");
}

TEST_F(ParseNodeGen, unary_functions){
    auto n = parse_node_p{new parse_node(Token::unary_function, f_square_root, {
        parse_node_p{new parse_node(Token::num_literal, 100)}
    })};
    EXPECT_EQ(generate(n), "SQR[100]");
}

TEST_F(ParseNodeGen, prg_name){
    auto b = parse_node_p{new parse_node(Token::block, {
        parse_node_p{new parse_node(Token::prg_name, std::string{"22156"})}
    })};
    EXPECT_EQ(generate(b), "O22156\n");
}

TEST_F(ParseNodeGen, simple_block){
    std::vector<parse_node_p> block = {
        parse_node_p{new parse_node(Token::n_word, 10)},
        parse_node_p{new parse_node(Token::g_word, g_linear_interpolation)},
        parse_node_p{new parse_node(Token::parameter, param_x, {
            parse_node_p{new parse_node(Token::num_int, 10)}
        })},
        parse_node_p{new parse_node(Token::parameter, param_z, {
            parse_node_p{new parse_node(Token::num_int, 20)}
        })},
        parse_node_p{new parse_node(Token::parameter, param_rpm, {
            parse_node_p{new parse_node(Token::num_int, 2000)}
        })},
        parse_node_p{new parse_node(Token::parameter, param_tool_selection, {
            parse_node_p{new parse_node(Token::string, std::string{"0202"})}
        })},
        parse_node_p{new parse_node(Token::m_word, m_activate_c_axis)},
        parse_node_p{new parse_node(Token::comment, std::string{"This is a test"})}
    };
    auto b = parse_node_p{new parse_node(Token::block, block)};
    EXPECT_EQ(generate(b), "N10 G1 X10 Z20 S2000 T0202 M17 (This is a test)\n");
}

TEST_F(ParseNodeGen, subsystem_select){
    auto b = parse_node_p{new parse_node(Token::block, std::vector<parse_node_p>{
        parse_node_p{new parse_node(Token::subsystem_select, 2)}
    })};
    EXPECT_EQ(generate(b), "$2\n");
}

TEST_F(ParseNodeGen, queueing){
    auto b = parse_node_p{new parse_node(Token::block, {
        parse_node_p{new parse_node(Token::queueing)},
    })};
    EXPECT_EQ(generate(b), "!\n");
}
