#include "fastxml/heap_xml_parser.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace fastxml;

TEST(HeapXmlTest, ParseSimpleXml) {
    using XmlT = HeapXmlNode;
    std::string xml_input = "<root attr=\"value\">Hello, world!<empty"
                            "/><empty attr=\"v\"/></root>";
    std::istringstream input(xml_input);

    auto root = parseSlow(input);
    //    auto root = root2.root();
    std::cout << root << "\n";

    //    EXPECT_TRUE(root2.file->isInFile(root.name()));
    EXPECT_EQ(root.type(), XmlT::Type::ELEMENT) << root;
    EXPECT_EQ(root.name(), "root") << root;

    const auto &attributes = root.attributes();

    ASSERT_EQ(attributes.size(), 1u);
    EXPECT_EQ(attributes.front().name, "attr");
    EXPECT_EQ(attributes.front().value, "value");

    EXPECT_EQ(root.begin()->content(), "Hello, world!") << root.content();
}

TEST(HeapXmlTest, ParseComplexXml) {
    using XmlT = HeapXmlNode;
    std::string xml_input = R"(
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
)";
    std::istringstream input(xml_input);

    auto root = parseSlow(input);

    std::cout << root << "\n";

    EXPECT_EQ(root.type(), XmlT::Type::ELEMENT);
    EXPECT_EQ(root.name(), "catalog");

    ASSERT_EQ(root.attributes().size(), 0u);

    auto book_iter = root.begin();
    ASSERT_NE(book_iter, root.end());
    EXPECT_EQ(book_iter->type(), XmlT::Type::ELEMENT);
    EXPECT_EQ(book_iter->name(), "book");

    const auto &book_attributes = book_iter->attributes();
    ASSERT_EQ(book_attributes.size(), 2u);
    EXPECT_EQ(book_attributes.at("id").str(), "bk101");
    EXPECT_EQ(book_attributes.at("edition").str(), "first");

    auto child_iter = book_iter->begin();
    ASSERT_NE(child_iter, book_iter->end());
    EXPECT_EQ(child_iter->type(), XmlT::Type::ELEMENT);
    EXPECT_EQ(child_iter->name(), "author");
    EXPECT_EQ(child_iter->begin()->content(), "Gambardella, Matthew");

    ++child_iter;
    ASSERT_NE(child_iter, book_iter->end());
    EXPECT_EQ(child_iter->type(), XmlT::Type::ELEMENT);
    EXPECT_EQ(child_iter->name(), "title");
    EXPECT_EQ(child_iter->begin()->content(), "XML Developer's Guide");

    // Add more assertions for the rest of the XML structure
}
