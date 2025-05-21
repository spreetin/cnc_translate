#include <gtest/gtest.h>
#include "../parser/parser.h"
#include "../generator/generator.h"

using namespace NCParser;

class Translation : public generator, public testing::Test{
public:
    Translation() : generator(Okuma, "OSP-P300L"), testing::Test() {}

protected:
};

TEST_F(Translation, short_single_subsystem){
    std::string input = "%\nO223221\nG1 X10 Z20 S3000\nG0 U5 W2\nU2 W20\nM30";
    auto p = new parser(Traub, "TX8H");
    p->parse(input);
    auto root = p->result();
    ASSERT_EQ(root->childCount(), 6);
    auto c0 = root->getChild(0);
    auto c1 = root->getChild(1);
    auto c2 = root->getChild(2);
    auto c3 = root->getChild(3);
    auto c4 = root->getChild(4);
    auto c5 = root->getChild(5);
    ASSERT_EQ(c0->childCount(), 1);
    EXPECT_EQ(c0->getChild(0)->type(), Token::percent);
    ASSERT_EQ(c1->childCount(), 1);
    EXPECT_EQ(c1->getChild(0)->type(), Token::prg_name);
    ASSERT_EQ(c2->childCount(), 4);
    ASSERT_EQ(c3->childCount(), 3);
    ASSERT_EQ(c4->childCount(), 3);
    ASSERT_EQ(c5->childCount(), 1);
    std::string output = generate(root);
    EXPECT_EQ(output, "%\nO223221\nG97 S3000 \nG1 X10 Z20 \nG91 \nG0 X5 Z2 \nG0 X2 Z20 \nG90 \nM30 \n");
}

