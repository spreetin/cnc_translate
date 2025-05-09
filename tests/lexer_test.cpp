#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <gtest/gtest.h>
#include "../lexer/lexer.h"
#include "../tokens/tokens.h"

class LexerTest : public NCParser::lexer, public testing::Test{
protected:
    int tokenizedReturn(int r) override {
        tokenized.push_back(r);
        iValues.push_back(iValue);
        dValues.push_back(fValue);
        sValues.push_back(sValue);
        return r;
    };

    std::vector<int> tokenized;
    std::vector<int> iValues;
    std::vector<double> dValues;
    std::vector<std::string> sValues;
};

TEST_F(LexerTest, Init){
    init("G100", {"EQ", "NE"});
}

TEST_F(LexerTest, Words){
    init("G100 M101", {});
    EXPECT_EQ(next(), NCParser::Token::g_word);
    EXPECT_EQ(intValue(), 100);
    EXPECT_EQ(next(), NCParser::Token::m_word);
    EXPECT_EQ(intValue(), 101);
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, Numbers){
    init("10.12 X100", {});
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(stringValue(), "10");
    EXPECT_EQ(next(), NCParser::Token::period);
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(stringValue(), "12");
    EXPECT_EQ(next(), NCParser::Token::x_word);
    EXPECT_EQ(next(), NCParser::Token::num_literal);
    EXPECT_EQ(stringValue(), "100");
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, Multiline){
    init("G100 EQ 100 NE FAIL", {"EQ", "NE"});
    next();
    EXPECT_EQ(next(), NCParser::Token::multi_letter);
    EXPECT_EQ(stringValue(), "EQ");
    next();
    EXPECT_EQ(next(), NCParser::Token::multi_letter);
    EXPECT_EQ(stringValue(), "NE");
    EXPECT_EQ(next(), NCParser::Token::unknown_function);
    EXPECT_EQ(stringValue(), "FAIL");
    EXPECT_EQ(next(), NCParser::Token::done);
}

TEST_F(LexerTest, ShortProgram){
    using namespace NCParser;
    std::string s = "%\r\nG97\n\nG1X100Y10.1\nM30\n%";
    std::vector<int> v = {Token::percent, Token::newline, Token::g_word, Token::newline,
        Token::newline, Token::g_word, Token::x_word, Token::num_literal, Token::y_word,
        Token::num_literal, Token::period, Token::num_literal, Token::newline,
        Token::m_word, Token::newline, Token::percent, Token::done};
    init(s, {});
    while (next() != Token::done);
    EXPECT_EQ(v, tokenized);
    EXPECT_EQ(iValues[2], 97);
    EXPECT_EQ(iValues[5], 1);
    EXPECT_EQ(sValues[7], "100");
    EXPECT_EQ(sValues[9], "10");
    EXPECT_EQ(sValues[11], "1");
    EXPECT_EQ(iValues[13], 30);
}

#endif // LEXER_TEST_H
