#include "fastxml/heapxmlnode.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace fastxml;

int main() {
    // Create some random number generators
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> tag_dist{0, 2};
    std::uniform_int_distribution<int> content_dist{0, 4};

    HeapXmlNode root{HeapXmlNode::Type::ELEMENT, "root"};

    for (int i = 0; i < 1000000; ++i) {
        // Determine the type of tag to create
        int tag_type = tag_dist(rng);

        if (tag_type == 0) {
            // Empty tag
            HeapXmlNode node{HeapXmlNode::Type::ELEMENT, "empty_tag"};
            root.add_child(std::move(node));
        }
        else if (tag_type == 1) {
            // Text content
            std::string content;
            int content_type = content_dist(rng);
            if (content_type == 0) {
                content =
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
            }
            else if (content_type == 1) {
                content = "Sed do eiusmod tempor incididunt ut labore et "
                          "dolore magna aliqua.";
            }
            else if (content_type == 2) {
                content =
                    "Ut enim ad minim veniam, quis nostrud exercitation "
                    "ullamco laboris nisi ut aliquip ex ea commodo consequat.";
            }
            else if (content_type == 3) {
                content = "Duis aute irure dolor in reprehenderit in voluptate "
                          "velit esse cillum dolore eu fugiat nulla pariatur.";
            }
            else if (content_type == 4) {
                content =
                    "Excepteur sint occaecat cupidatat non proident, sunt in "
                    "culpa qui officia deserunt mollit anim id est laborum.";
            }

            HeapXmlNode node{
                HeapXmlNode::Type::TEXT_CONTENT, "", {}, {}, content};
            root.add_child(std::move(node));
        }
        else if (tag_type == 2) {
            // Element with attributes
            HeapXmlNode node{HeapXmlNode::Type::ELEMENT,
                             "element_with_attributes"};
            node.attributes().push_back({"attr1", "value1"});
            node.attributes().push_back({"attr2", "value2"});
            root.add_child(std::move(node));
        }
    }

    //    std::cout << root << "\n";
    std::ofstream{"generated2.json"} << root << "\n";
}
