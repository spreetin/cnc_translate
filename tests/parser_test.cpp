#include <gtest/gtest.h>
#include "../parser/parser.h"
#include "../tokens/tokens.h"

using namespace NCParser;

class Parser : public parser, public testing::Test{
public:
    Parser() : parser(NCParser::Traub, "TX8H"), testing::Test() {}

protected:
};

TEST_F(Parser, init){
    EXPECT_TRUE(param.general.has_nose_radius_compenstion);
    EXPECT_EQ(param.g[g_circular_interpolation_ccw].parameter_types['C'], param_transfer_radius);
    EXPECT_TRUE(allowed_multiletter.contains("GOTO"));
    EXPECT_TRUE(param.functions.unary.contains("SQR"));
    EXPECT_TRUE(param.functions.binary.contains("EQ"));
}

TEST_F(Parser, morefuncs){
    auto p1 = std::make_shared<parse_node>(Token::num_int);
    m_lexer->init("EQ 2", allowed_multiletter);
    next = m_lexer->next();
    auto p2 = morefuncs(p1);
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p2);
    EXPECT_EQ(p2->type(), Token::binary_functions);
    EXPECT_EQ(p2->intValue(), f_comparison_equal);
    EXPECT_EQ(p2->childCount(), 2);
    EXPECT_EQ(p2->getChild(1)->intValue(), 2);
}

TEST_F(Parser, func_number){
    m_lexer->init("100", allowed_multiletter);
    next = m_lexer->next();
    auto p = func();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::num_int);
    EXPECT_EQ(p->intValue(), 100);
}

TEST_F(Parser, func_parenthesis){
    m_lexer->init("[100]", allowed_multiletter);
    next = m_lexer->next();
    auto p = func();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::num_int);
    EXPECT_EQ(p->intValue(), 100);
}

TEST_F(Parser, func_unary){
    m_lexer->init("SQR[10]", allowed_multiletter);
    next = m_lexer->next();
    auto p = func();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::unary_function);
    EXPECT_EQ(p->intValue(), f_square_root);
    EXPECT_EQ(p->childCount(), 1);
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 10);
}

TEST_F(Parser, func_variable){
    m_lexer->init("L102", allowed_multiletter);
    next = m_lexer->next();
    auto p = func();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::variable);
    EXPECT_EQ(p->intValue(), 102);
}

TEST_F(Parser, morefactors){
    auto p1 = std::make_shared<parse_node>(Token::num_int, 100);
    m_lexer->init(" * 45", allowed_multiletter);
    next = m_lexer->next();
    auto p2 = morefactors(p1);
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p2);
    EXPECT_EQ(p2->type(), Token::star);
    EXPECT_EQ(p2->childCount(), 2);
    EXPECT_EQ(p2->getChild(1)->intValue(), 45);
}

TEST_F(Parser, factor){
    // Binary operator
    m_lexer->init("100 EQ 2", allowed_multiletter);
    next = m_lexer->next();
    auto p = factor();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::binary_functions);
    EXPECT_EQ(p->intValue(), f_comparison_equal);
    EXPECT_EQ(p->childCount(), 2);
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 100);
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 2);
}

TEST_F(Parser, moreterms){
    auto p1 = std::make_shared<parse_node>(Token::num_int, 100);
    m_lexer->init(" - 45", allowed_multiletter);
    next = m_lexer->next();
    auto p2 = moreterms(p1);
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p2);
    EXPECT_EQ(p2->type(), Token::minus);
    EXPECT_EQ(p2->childCount(), 2);
    EXPECT_EQ(p2->getChild(1)->intValue(), 45);
}

TEST_F(Parser, term){
    // Multiply
    m_lexer->init("100 * 2", allowed_multiletter);
    next = m_lexer->next();
    auto p = term();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::star);
    EXPECT_EQ(p->childCount(), 2);
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 100);
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 2);
}

TEST_F(Parser, comment){
    m_lexer->init("(This is a test!)\n", allowed_multiletter);
    next = m_lexer->next();
    auto p = comment();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(next, Token::newline);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->type(), Token::comment);
    EXPECT_EQ(p->stringValue(), "This is a test!");
}

TEST_F(Parser, g_continuing){
    m_lexer->init("G1 X100 T0202\nX20 Z10", allowed_multiletter);
    next = m_lexer->next();

    // First G1
    auto p = g();
    ASSERT_EQ(errors.size(), 0);
    EXPECT_EQ(next, Token::newline);
    ASSERT_EQ(p.size(), 3);
    EXPECT_EQ(p[0]->type(), Token::g_word);
    EXPECT_EQ(p[0]->intValue(), g_linear_interpolation);
    EXPECT_EQ(p[1]->type(), Token::parameter);
    int xParam;
    if (param.g[g_linear_interpolation].parameter_types.contains('X')){
        xParam = param.g[g_linear_interpolation].parameter_types['X'];
    } else {
        xParam = param.parameter.defaults['X'];
    }
    int tParam;
    if (param.g[g_linear_interpolation].parameter_types.contains('T')){
        tParam = param.g[g_linear_interpolation].parameter_types['T'];
    } else {
        tParam = param.parameter.defaults['T'];
    }
    EXPECT_EQ(p[1]->intValue(), xParam);
    EXPECT_EQ(p[1]->childCount(), 1);
    EXPECT_EQ(p[1]->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p[1]->getChild(0)->intValue(), 100);
    EXPECT_EQ(p[2]->type(), Token::parameter);
    EXPECT_EQ(p[2]->intValue(), tParam);
    EXPECT_EQ(p[2]->childCount(), 1);
    EXPECT_EQ(p[2]->getChild(0)->type(), Token::num_literal);
    EXPECT_EQ(p[2]->getChild(0)->stringValue(), "0202");
    match(Token::newline);

    // Second G1
    p = g(true);
    ASSERT_EQ(errors.size(), 0);
    EXPECT_EQ(next, Token::done);
    ASSERT_EQ(p.size(), 3);
    EXPECT_EQ(p[0]->type(), Token::g_word);
    EXPECT_EQ(p[0]->intValue(), g_linear_interpolation);
    EXPECT_EQ(p[1]->type(), Token::parameter);
    int zParam;
    if (param.g[g_linear_interpolation].parameter_types.contains('Z')){
        zParam = param.g[g_linear_interpolation].parameter_types['Z'];
    } else {
        zParam = param.parameter.defaults['Z'];
    }
    EXPECT_EQ(p[1]->intValue(), xParam);
    EXPECT_EQ(p[1]->childCount(), 1);
    EXPECT_EQ(p[1]->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p[1]->getChild(0)->intValue(), 20);
    EXPECT_EQ(p[2]->type(), Token::parameter);
    EXPECT_EQ(p[2]->intValue(), zParam);
    EXPECT_EQ(p[2]->childCount(), 1);
    EXPECT_EQ(p[2]->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p[2]->getChild(0)->intValue(), 10);
}

TEST_F(Parser, number_int){
    // Int
    m_lexer->init("301", allowed_multiletter);
    next = m_lexer->next();
    auto p = number();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::num_int);
    EXPECT_EQ(p->intValue(), 301);

    // Float
    m_lexer->init("100.2", allowed_multiletter);
    next = m_lexer->next();
    p = number();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::num_float);
    EXPECT_EQ(p->doubleValue(), 100.2);
}

TEST_F(Parser, number_float){

}

TEST_F(Parser, variable){
    m_lexer->init("L301", allowed_multiletter);
    next = m_lexer->next();
    auto p = variable();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::variable);
    EXPECT_EQ(p->intValue(), 301);
}

TEST_F(Parser, assignment){
    m_lexer->init("L301 = 2", allowed_multiletter);
    next = m_lexer->next();
    auto p = assignment();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::equals);
    EXPECT_EQ(p->childCount(), 2);
    ASSERT_TRUE(p->getChild(0));
    EXPECT_EQ(p->getChild(0)->type(), Token::variable);
    ASSERT_TRUE(p->getChild(1));
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 2);
}

TEST_F(Parser, expr_number){
    m_lexer->init("256", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::num_int);
    EXPECT_EQ(p->intValue(), 256);
}

TEST_F(Parser, expr_variable){
    // Variable
    m_lexer->init("L200", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::variable);
    EXPECT_EQ(p->intValue(), 200);
}

TEST_F(Parser, expr_term){
    // Addition
    m_lexer->init("5 - 2", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::minus);
    ASSERT_EQ(p->childCount(), 2);
    ASSERT_TRUE(p->getChild(0));
    ASSERT_TRUE(p->getChild(1));
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 5);
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 2);
}

TEST_F(Parser, expr_factor){
    // Division
    m_lexer->init("5 / 2", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::slash);
    ASSERT_EQ(p->childCount(), 2);
    ASSERT_TRUE(p->getChild(0));
    ASSERT_TRUE(p->getChild(1));
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 5);
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 2);
}

TEST_F(Parser, expr_unary){
    // Unary function
    m_lexer->init("SQR[9]", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::unary_function);
    EXPECT_EQ(p->intValue(), param.functions.unary["SQR"]);
    ASSERT_EQ(p->childCount(), 1);
    ASSERT_TRUE(p->getChild(0));
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 9);
}

TEST_F(Parser, expr_binary){
    // Binary function
    m_lexer->init("2 EQ 3", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::binary_functions);
    EXPECT_EQ(p->intValue(), param.functions.binary["EQ"]);
    ASSERT_EQ(p->childCount(), 2);
    ASSERT_TRUE(p->getChild(0));
    ASSERT_TRUE(p->getChild(1));
    EXPECT_EQ(p->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(0)->intValue(), 2);
    EXPECT_EQ(p->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(p->getChild(1)->intValue(), 3);
}

TEST_F(Parser, expr_parenthesis){
    m_lexer->init("[2 + 4] * 3", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::star);
    ASSERT_EQ(p->childCount(), 2);

    // [2 + 4]
    auto c0 = p->getChild(0);
    ASSERT_TRUE(c0);
    EXPECT_EQ(c0->type(), Token::plus);
    ASSERT_EQ(c0->childCount(), 2);
    ASSERT_TRUE(c0->getChild(0));
    ASSERT_TRUE(c0->getChild(1));
    EXPECT_EQ(c0->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(c0->getChild(0)->intValue(), 2);
    EXPECT_EQ(c0->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(c0->getChild(1)->intValue(), 4);

    // 3
    auto c1 = p->getChild(1);
    ASSERT_TRUE(c1);
    EXPECT_EQ(c1->type(), Token::num_int);
    EXPECT_EQ(c1->intValue(), 3);
}

TEST_F(Parser, expr_oop){
    // Should be: 2 + (4 * 3)
    m_lexer->init("2 + 4 * 3", allowed_multiletter);
    next = m_lexer->next();
    auto p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::plus);
    ASSERT_EQ(p->childCount(), 2);

    // 2
    auto c0 = p->getChild(0);
    ASSERT_TRUE(c0);
    EXPECT_EQ(c0->type(), Token::num_int);
    EXPECT_EQ(c0->intValue(), 2);

    // 4 * 3
    auto c1 = p->getChild(1);
    ASSERT_TRUE(c1);
    EXPECT_EQ(c1->type(), Token::star);
    ASSERT_EQ(c1->childCount(), 2);
    ASSERT_TRUE(c1->getChild(0));
    ASSERT_TRUE(c1->getChild(1));
    EXPECT_EQ(c1->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(c1->getChild(0)->intValue(), 4);
    EXPECT_EQ(c1->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(c1->getChild(1)->intValue(), 3);

    // Should be: (2 * 3) + (4 * (SIN[3]))
    m_lexer->init("2 * 3 + 4 * SIN[3]", allowed_multiletter);
    next = m_lexer->next();
    p = expr();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::plus);
    ASSERT_EQ(p->childCount(), 2);

    // 2 * 3
    c0 = p->getChild(0);
    ASSERT_TRUE(c0);
    EXPECT_EQ(c0->type(), Token::star);
    ASSERT_EQ(c0->childCount(), 2);
    ASSERT_TRUE(c0->getChild(0));
    ASSERT_TRUE(c0->getChild(1));
    EXPECT_EQ(c0->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(c0->getChild(0)->intValue(), 2);
    EXPECT_EQ(c0->getChild(1)->type(), Token::num_int);
    EXPECT_EQ(c0->getChild(1)->intValue(), 3);

    // 4 * 3
    c1 = p->getChild(1);
    ASSERT_TRUE(c1);
    EXPECT_EQ(c1->type(), Token::star);
    ASSERT_EQ(c1->childCount(), 2);
    auto c1_1 = c1->getChild(1);
    ASSERT_TRUE(c1->getChild(0));
    ASSERT_TRUE(c1_1);
    EXPECT_EQ(c1->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(c1->getChild(0)->intValue(), 4);
    EXPECT_EQ(c1_1->type(), Token::unary_function);
    EXPECT_EQ(c1_1->intValue(), param.functions.unary["SIN"]);

    ASSERT_EQ(c1_1->childCount(), 1);
    ASSERT_TRUE(c1_1->getChild(0));
    EXPECT_EQ(c1_1->getChild(0)->type(), Token::num_int);
    EXPECT_EQ(c1_1->getChild(0)->intValue(), 3);
}

TEST_F(Parser, block){
    m_lexer->init("G0 X100 Z20 S2000 T0303 M17\nG1 U20 W10\nU10W1", allowed_multiletter);
    next = m_lexer->next();

    // Line one
    auto p = block();
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 6);

    auto c0 = p->getChild(0);
    auto c1 = p->getChild(1);
    auto c2 = p->getChild(2);
    auto c3 = p->getChild(3);
    auto c4 = p->getChild(4);
    auto c5 = p->getChild(5);

    ASSERT_TRUE(c0 && c1 && c2 && c3 && c4 && c5);

    EXPECT_EQ(c0->type(), Token::g_word);
    EXPECT_EQ(c1->type(), Token::parameter);
    EXPECT_EQ(c2->type(), Token::parameter);
    EXPECT_EQ(c3->type(), Token::parameter);
    EXPECT_EQ(c4->type(), Token::parameter);
    EXPECT_EQ(c5->type(), Token::m_word);

    int g = active_g_numbers[0];
    EXPECT_EQ(c0->intValue(), g);
    int pType;
    if (param.g[g].parameter_types.contains('X')){
        pType = param.g[g].parameter_types['X'];
    } else {
        pType = param.parameter.defaults['X'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('Z')){
        pType = param.g[g].parameter_types['Z'];
    } else {
        pType = param.parameter.defaults['Z'];
    }
    EXPECT_EQ(c2->intValue(), pType);
    if (param.g[g].parameter_types.contains('S')){
        pType = param.g[g].parameter_types['S'];
    } else {
        pType = param.parameter.defaults['S'];
    }
    EXPECT_EQ(c3->intValue(), pType);
    if (param.g[g].parameter_types.contains('T')){
        pType = param.g[g].parameter_types['T'];
    } else {
        pType = param.parameter.defaults['T'];
    }
    EXPECT_EQ(c4->intValue(), pType);
    EXPECT_EQ(c5->intValue(), active_m_numbers[17]);

    // Line two
    p = block();
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 3);

    c0 = p->getChild(0);
    c1 = p->getChild(1);
    c2 = p->getChild(2);

    g = active_g_numbers[1];
    EXPECT_EQ(c0->intValue(), g);
    if (param.g[g].parameter_types.contains('U')){
        pType = param.g[g].parameter_types['U'];
    } else {
        pType = param.parameter.defaults['U'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('W')){
        pType = param.g[g].parameter_types['W'];
    } else {
        pType = param.parameter.defaults['W'];
    }
    EXPECT_EQ(c2->intValue(), pType);

    // Line three
    p = block();
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 3);

    c0 = p->getChild(0);
    c1 = p->getChild(1);
    c2 = p->getChild(2);

    g = active_g_numbers[1];
    EXPECT_EQ(c0->intValue(), g);
    if (param.g[g].parameter_types.contains('U')){
        pType = param.g[g].parameter_types['U'];
    } else {
        pType = param.parameter.defaults['U'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('W')){
        pType = param.g[g].parameter_types['W'];
    } else {
        pType = param.parameter.defaults['W'];
    }
    EXPECT_EQ(c2->intValue(), pType);
}

TEST_F(Parser, list){
    m_lexer->init("G0 X100 Z20 S2000 T0303 M17\nG1 U20 W10\nU10W1", allowed_multiletter);
    next = m_lexer->next();

    // Line one
    auto l = list();
    ASSERT_EQ(l.size(), 3);

    auto p = l.at(0);
    ASSERT_EQ(errors.size(), 0);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 6);

    auto c0 = p->getChild(0);
    auto c1 = p->getChild(1);
    auto c2 = p->getChild(2);
    auto c3 = p->getChild(3);
    auto c4 = p->getChild(4);
    auto c5 = p->getChild(5);

    ASSERT_TRUE(c0 && c1 && c2 && c3 && c4 && c5);

    EXPECT_EQ(c0->type(), Token::g_word);
    EXPECT_EQ(c1->type(), Token::parameter);
    EXPECT_EQ(c2->type(), Token::parameter);
    EXPECT_EQ(c3->type(), Token::parameter);
    EXPECT_EQ(c4->type(), Token::parameter);
    EXPECT_EQ(c5->type(), Token::m_word);

    int g = active_g_numbers[0];
    EXPECT_EQ(c0->intValue(), g);
    int pType;
    if (param.g[g].parameter_types.contains('X')){
        pType = param.g[g].parameter_types['X'];
    } else {
        pType = param.parameter.defaults['X'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('Z')){
        pType = param.g[g].parameter_types['Z'];
    } else {
        pType = param.parameter.defaults['Z'];
    }
    EXPECT_EQ(c2->intValue(), pType);
    if (param.g[g].parameter_types.contains('S')){
        pType = param.g[g].parameter_types['S'];
    } else {
        pType = param.parameter.defaults['S'];
    }
    EXPECT_EQ(c3->intValue(), pType);
    if (param.g[g].parameter_types.contains('T')){
        pType = param.g[g].parameter_types['T'];
    } else {
        pType = param.parameter.defaults['T'];
    }
    EXPECT_EQ(c4->intValue(), pType);
    EXPECT_EQ(c5->intValue(), active_m_numbers[17]);

    // Line two
    p = l.at(1);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 3);

    c0 = p->getChild(0);
    c1 = p->getChild(1);
    c2 = p->getChild(2);

    g = active_g_numbers[1];
    EXPECT_EQ(c0->intValue(), g);
    if (param.g[g].parameter_types.contains('U')){
        pType = param.g[g].parameter_types['U'];
    } else {
        pType = param.parameter.defaults['U'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('W')){
        pType = param.g[g].parameter_types['W'];
    } else {
        pType = param.parameter.defaults['W'];
    }
    EXPECT_EQ(c2->intValue(), pType);

    // Line three
    p = l.at(2);
    ASSERT_TRUE(p);
    EXPECT_EQ(p->type(), Token::block);
    ASSERT_EQ(p->childCount(), 3);

    c0 = p->getChild(0);
    c1 = p->getChild(1);
    c2 = p->getChild(2);

    g = active_g_numbers[1];
    EXPECT_EQ(c0->intValue(), g);
    if (param.g[g].parameter_types.contains('U')){
        pType = param.g[g].parameter_types['U'];
    } else {
        pType = param.parameter.defaults['U'];
    }
    EXPECT_EQ(c1->intValue(), pType);
    if (param.g[g].parameter_types.contains('W')){
        pType = param.g[g].parameter_types['W'];
    } else {
        pType = param.parameter.defaults['W'];
    }
    EXPECT_EQ(c2->intValue(), pType);
}

TEST_F(Parser, program){
    // TODO Add a full NC program to test parsing on
}

