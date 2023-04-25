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
        //        std::cout << "Type: " << static_cast<int>(token.type())
        //                  << ", Content: " << token.str() << ", Line: " <<
        //                  token.line()
        //                  << ", Column: " << token.col() << std::endl;
        std::cout << token;
    }

    ASSERT_EQ(tokens.size(), expected_tokens.size());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type(), expected_tokens[i].type());
        EXPECT_EQ(tokens[i].str(), expected_tokens[i].str());
        //        EXPECT_EQ(tokens[i].line(), expected_tokens[i].line());
        //        EXPECT_EQ(tokens[i].col(), expected_tokens[i].col());
    }
}

TEST(XmlParserTest, MoreComplexExample) {
    std::istringstream input(R"(
<catalog>
    <book id="bk101" edition="first">
        <author>Gambardella, Matthew</author>
        <title>XML Developer's Guide</title>
        <genre>Computer</genre>
        <price>44.95</price>
        <publish_date>2000-10-01</publish_date>
        <description>An in-depth look at creating applications with XML.</description>
    </book>
    <book id="bk102" edition="second">
        <author>Ralls, Kim</author>
        <title>Midnight Rain</title>
        <genre>Fantasy</genre>
        <price>5.95</price>
        <publish_date>2000-12-16</publish_date>
        <description>A former architect battles corporate zombies, an evil sorceress, and her own childhood to become queen of the world.</description>
    </book>
</catalog>
)");

    std::vector<XmlToken> tokens = tokenize(input);

    std::vector<XmlToken> expected_tokens = {
        XmlToken(TokenType::ELEMENT_OPEN, "catalog", 2, 1),
        XmlToken(TokenType::ELEMENT_OPEN, "book", 3, 5),
        XmlToken(TokenType::ATTRIBUTE_NAME, "id", 3, 10),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "bk101", 3, 14),
        XmlToken(TokenType::ATTRIBUTE_NAME, "edition", 3, 20),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "first", 3, 29),
        XmlToken(TokenType::ELEMENT_OPEN, "author", 4, 9),
        XmlToken(TokenType::TEXT_CONTENT, "Gambardella, Matthew", 4, 16),
        XmlToken(TokenType::ELEMENT_CLOSE, "", 4, 36),
        XmlToken(TokenType::ELEMENT_OPEN, "title", 5, 9),
        XmlToken(TokenType::TEXT_CONTENT, "XML Developer's Guide", 5, 15),
        XmlToken(TokenType::ELEMENT_CLOSE, "", 5, 37),
        // ...
        // Add the rest of the tokens here
    };

    // ASSERT_EQ(tokens.size(), expected_tokens.size());

    std::cout << "Received tokens:" << std::endl;
    {
        int i = 0;
        for (const auto &token : tokens) {
            std::cout << i << ": " << token << "\n";
            ++i;
        }
    }

    for (size_t i = 0; i < tokens.size() && i < expected_tokens.size(); ++i) {
        EXPECT_EQ(tokens.at(i), expected_tokens.at(i))
            << "Failure at index " << i << ".";
        //        EXPECT_EQ(tokens[i].str(), expected_tokens[i].str())
        //            << "Failure at index " << i << ".";
        // EXPECT_EQ(tokens[i].line(), expected_tokens[i].line());
        // EXPECT_EQ(tokens[i].col(), expected_tokens[i].col());
    }
}
