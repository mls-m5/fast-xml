// xml_parser_test.cpp
#include "xml_parser.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(XmlParserTest, TokenizeBasicXml) {
    std::string xml_input = "<root attr=\"value\">Hello, world!</root>";
    std::istringstream input(xml_input);
    std::vector<XmlToken> tokens = tokenize(input);

    std::vector<XmlToken> expected_tokens = {
        XmlToken(TokenType::ELEMENT_OPEN, "root", 1, 1),
        XmlToken(TokenType::ATTRIBUTE_NAME, "attr", 1, 7),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "value", 1, 13),
        XmlToken(TokenType::TEXT_CONTENT, "Hello, world!", 1, 20),
        XmlToken(TokenType::ELEMENT_CLOSE, "", 1, 34)};

    // Output the received tokens for debugging
    std::cout << "Received tokens:" << std::endl;
    for (const auto &token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type())
                  << ", Content: " << token.str() << ", Line: " << token.line()
                  << ", Column: " << token.col() << std::endl;
    }

    ASSERT_EQ(tokens.size(), expected_tokens.size());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type(), expected_tokens[i].type());
        EXPECT_EQ(tokens[i].str(), expected_tokens[i].str());
        //        EXPECT_EQ(tokens[i].line(), expected_tokens[i].line());
        //        EXPECT_EQ(tokens[i].col(), expected_tokens[i].col());
    }
}
