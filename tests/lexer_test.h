#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <gtest/gtest.h>
#include "../lexer/lexer.h"

class ParserTester : public NCParser::lexer, public testing::Test{};


#endif // LEXER_TEST_H
