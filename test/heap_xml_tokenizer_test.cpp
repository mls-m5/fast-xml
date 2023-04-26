// xml_parser_test.cpp
#include "fastxml/heap_xml_tokenizer.h"
#include "fastxml/heapxmltoken.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace fastxml;

using XmlToken = HeapXmlToken;

TEST(XmlParserTest, TokenizeBasicXml) {
    std::string xml_input =
        "<root attr=\"value\">Hello, world!<empty attr=\"value\"/></root>";
    std::istringstream input(xml_input);
    std::vector<XmlToken> tokens = tokenize_on_heap(input);

    std::vector<XmlToken> expected_tokens = {
        XmlToken(TokenType::ELEMENT_OPEN, "root", 1, 1),
        XmlToken(TokenType::ATTRIBUTE_NAME, "attr", 1, 7),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "value", 1, 13),
        XmlToken(TokenType::TEXT_CONTENT, "Hello, world!", 1, 20),
        XmlToken(TokenType::ELEMENT_OPEN, "empty", 1, 20),
        XmlToken(TokenType::ATTRIBUTE_NAME, "attr", 1, 7),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "value", 1, 13),
        XmlToken(TokenType::ELEMENT_CLOSE, "", 1, 13),
        XmlToken(TokenType::ELEMENT_CLOSE, "root", 1, 34)};

    // Output the received tokens for debugging
    std::cout << "Received tokens:" << std::endl;
    for (const auto &token : tokens) {
        std::cout << token << "\n";
    }

    ASSERT_EQ(tokens.size(), expected_tokens.size());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
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

    std::vector<XmlToken> tokens = tokenize_on_heap(input);

    std::vector<XmlToken> expected_tokens = {
        XmlToken(TokenType::ELEMENT_OPEN, "catalog", 2, 1),
        XmlToken(TokenType::ELEMENT_OPEN, "book", 3, 5),
        XmlToken(TokenType::ATTRIBUTE_NAME, "id", 3, 10),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "bk101", 3, 14),
        XmlToken(TokenType::ATTRIBUTE_NAME, "edition", 3, 20),
        XmlToken(TokenType::ATTRIBUTE_VALUE, "first", 3, 29),
        XmlToken(TokenType::ELEMENT_OPEN, "author", 4, 9),
        XmlToken(TokenType::TEXT_CONTENT, "Gambardella, Matthew", 4, 16),
        XmlToken(TokenType::ELEMENT_CLOSE, "author", 4, 36),
        XmlToken(TokenType::ELEMENT_OPEN, "title", 5, 9),
        XmlToken(TokenType::TEXT_CONTENT, "XML Developer's Guide", 5, 15),
        XmlToken(TokenType::ELEMENT_CLOSE, "title", 5, 37),
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
    }
}
