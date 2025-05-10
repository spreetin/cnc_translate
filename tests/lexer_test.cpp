#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <gtest/gtest.h>
#include "../lexer/lexer.h"
#include "../tokens/tokens.h"

class LexerTest : public NCParser::lexer, public testing::Test{
protected:
    int tokenizedReturn(int r) override {
        tokenized.push_back(r);
        values.push_back(vValue);
        return r;
    };

    std::vector<int> tokenized;
    std::vector<std::variant<int,double,std::string>> values;
};

TEST_F(LexerTest, Init){
    init("G100", {"EQ", "NE"});
}

TEST_F(LexerTest, Words){
    init("G100 M101");
    EXPECT_EQ(next(), NCParser::Token::g_word) << text.at(m_pos);
    EXPECT_EQ(int_value(), 100);
    EXPECT_EQ(next(), NCParser::Token::m_word);
    EXPECT_EQ(int_value(), 101);
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, Numbers){
    init("10.12 X100");
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(string_value(), "10");
    EXPECT_EQ(next(), NCParser::Token::period);
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(string_value(), "12");
    EXPECT_EQ(next(), NCParser::Token::x_word);
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(string_value(), "100");
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, Multiline){
    init("G100 EQ 100 NE FAIL", {"EQ", "NE"});
    next();
    EXPECT_EQ(next(), NCParser::Token::multi_letter);
    EXPECT_EQ(string_value(), "EQ");
    next();
    EXPECT_EQ(next(), NCParser::Token::multi_letter);
    EXPECT_EQ(string_value(), "NE");
    EXPECT_EQ(next(), NCParser::Token::unknown_function);
    EXPECT_EQ(string_value(), "FAIL");
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, ShortProgram){
    using namespace NCParser;
    std::string s = "%\r\nG97\n\nG1X100Y10.1\nM30\n%";
    std::vector<int> v = {Token::percent, Token::newline, Token::g_word, Token::newline,
        Token::newline, Token::g_word, Token::x_word, Token::num_literal, Token::y_word,
        Token::num_literal, Token::period, Token::num_literal, Token::newline,
        Token::m_word, Token::newline, Token::percent, Token::done};
    init(s);
    while (next() != Token::done);
    EXPECT_EQ(v, tokenized);
    ASSERT_EQ(values[2].index(), 0);
    EXPECT_EQ(std::get<int>(values[2]), 97);
    ASSERT_EQ(values[5].index(), 0);
    EXPECT_EQ(std::get<int>(values[5]), 1);
    ASSERT_EQ(values[7].index(), 2);
    EXPECT_EQ(std::get<std::string>(values[7]), "100");
    ASSERT_EQ(values[9].index(), 2);
    EXPECT_EQ(std::get<std::string>(values[9]), "10");
    ASSERT_EQ(values[11].index(), 2);
    EXPECT_EQ(std::get<std::string>(values[11]), "1");
    ASSERT_EQ(values[13].index(), 0);
    EXPECT_EQ(std::get<int>(values[13]), 30);
}

#endif // LEXER_TEST_H
