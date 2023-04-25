// xml_parser_test.cpp
#include "xml_parser.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(TokenizeTest, BasicTest) {
    std::istringstream xml_input(
        "<element attr=\"value\">Text content</element>");

    std::vector<XmlToken> expected_tokens = {
        XmlToken(TokenType::ELEMENT_OPEN, "", 1, 1),
        XmlToken(TokenType::ATTRIBUTE_NAME, "attr", 1, 10),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "value", 1, 15),
        XmlToken(TokenType::TEXT_CONTENT, "Text content", 1, 22),
        XmlToken(TokenType::ELEMENT_CLOSE, "", 1, 34)};

    auto tokens = tokenize(xml_input);

    ASSERT_EQ(tokens.size(), expected_tokens.size());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].str(), expected_tokens[i].str());
        EXPECT_EQ(tokens[i].line(), expected_tokens[i].line());
        EXPECT_EQ(tokens[i].col(), expected_tokens[i].col());
    }
}
