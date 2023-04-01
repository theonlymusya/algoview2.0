#pragma once
#include <fstream>
#include "include/rapidjson/document.h"

namespace algoview_xml_parser {

using namespace rapidjson;

class XML_Parser {
   public:
    void open_file();
    void xml_to_json();
    void print_json();
    void parse_json_to_Document();
    void parse();
    const Document& get_DOM_tree();

   private:
    std::ifstream file_;
    std::string json_str_;
    Document DOM_tree_;
};
}  // namespace algoview_xml_parser
