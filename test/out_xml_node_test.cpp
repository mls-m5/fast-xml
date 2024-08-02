#include "fastxml/outxmlnode.h"
#include "fastxml/xml_parser.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace fastxml;

TEST(XmlTest, BasicOutputTest) {
    using XmlT = OutXmlNode;
    std::ostringstream ss;
    {
        auto out = OutXmlNode{ss, "root"};

        {
            auto child = out.add_child("hello");
            child.add_attribute("width", "10");
            child.content("hello there");
        }
    }

    auto xml = parse(ss.str());

    std::cerr << "bajs\n";
    std::cerr << ss.str() << std::endl;

    ASSERT_EQ(xml->root().name(), "root");

    ASSERT_EQ(xml->root().begin()->name(), "hello");
    ASSERT_EQ(xml->root().begin()->attr("width").value, "10");

    std::cout << ss.str() << "\n";
}
