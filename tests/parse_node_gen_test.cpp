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
    auto n = std::make_shared<parse_node>(Token::num_int, 123);
    EXPECT_EQ(generate(n), "123");
}

TEST_F(ParseNodeGen, variable){
    auto n = std::make_shared<parse_node>(Token::variable, 501);
    EXPECT_EQ(generate(n), "L501");
}

TEST_F(ParseNodeGen, equals){
    auto n = std::make_shared<parse_node>(Token::equals, -1, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "100 = 100");
}

TEST_F(ParseNodeGen, star){
    auto n = std::make_shared<parse_node>(Token::star, -1, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "[100 * 100]");
}

TEST_F(ParseNodeGen, plus){
    auto n = std::make_shared<parse_node>(Token::plus, -1, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "[100 + 100]");
}

TEST_F(ParseNodeGen, minus){
    auto n = std::make_shared<parse_node>(Token::minus, -1, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "[100 - 100]");
}

TEST_F(ParseNodeGen, slash){
    auto n = std::make_shared<parse_node>(Token::slash, -1, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "[100 / 100]");
}

TEST_F(ParseNodeGen, binary_functions){
    auto n = std::make_shared<parse_node>(Token::binary_functions, f_comparison_greater, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100),
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "[100 GT 100]");
}

TEST_F(ParseNodeGen, unary_functions){
    auto n = std::make_shared<parse_node>(Token::unary_function, f_square_root, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::num_literal, 100)
    });
    EXPECT_EQ(generate(n), "SQR[100]");
}

TEST_F(ParseNodeGen, prg_name){
    auto b = std::make_shared<parse_node>(Token::block, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::prg_name, std::string{"22156"})
    });
    EXPECT_EQ(generate(b), "O22156\n");
}

TEST_F(ParseNodeGen, simple_block){
    std::vector<parse_node_p> block = {
        std::make_shared<parse_node>(Token::n_word, 10),
        std::make_shared<parse_node>(Token::g_word, g_linear_interpolation),
        std::make_shared<parse_node>(Token::parameter, param_x, std::vector<parse_node_p>{
            std::make_shared<parse_node>(Token::num_int, 10)
        }),
        std::make_shared<parse_node>(Token::parameter, param_z, std::vector<parse_node_p>{
            std::make_shared<parse_node>(Token::num_int, 20)
        }),
        std::make_shared<parse_node>(Token::parameter, param_rpm, std::vector<parse_node_p>{
            std::make_shared<parse_node>(Token::num_int, 2000)
        }),
        std::make_shared<parse_node>(Token::parameter, param_tool_selection, std::vector<parse_node_p>{
            std::make_shared<parse_node>(Token::string, std::string{"0202"})
        }),
        std::make_shared<parse_node>(Token::m_word, m_activate_c_axis),
        std::make_shared<parse_node>(Token::comment, std::string{"This is a test"})
    };
    auto b = std::make_shared<parse_node>(Token::block, block);
    EXPECT_EQ(generate(b), "N10 G1 X10 Z20 S2000 T0202 M17 (This is a test)\n");
}

TEST_F(ParseNodeGen, subsystem_select){
    auto b = std::make_shared<parse_node>(Token::block, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::subsystem_select, 2)
    });
    EXPECT_EQ(generate(b), "$2\n");
}

TEST_F(ParseNodeGen, queueing){
    auto b = std::make_shared<parse_node>(Token::block, std::vector<parse_node_p>{
        std::make_shared<parse_node>(Token::queueing),
    });
    EXPECT_EQ(generate(b), "!\n");
}
