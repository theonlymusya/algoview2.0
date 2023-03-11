#include "xml_parser.hpp"
#include <iostream>
#include <sstream>
#include "include/rapidjson/document.h"
#include "include/xml2json.hpp"

namespace algoview_xml_parser {

using namespace rapidjson;

void XML_Parser::open_file() {
    file.open("test1.xml");
}

void XML_Parser::xml_to_json() {
    std::ostringstream oss;
    oss << file.rdbuf();
    json_str = xml2json(oss.str().data());
}

void XML_Parser::print_json() {
    std::cout << json_str << std::endl;
}

void XML_Parser::parse_json_to_Document() {
    // std::cout << "Ready to parse json" << std::endl;
    const char* json_cstr = json_str.c_str();
    assert(!DOM_tree.Parse(json_cstr).HasParseError() && "Ошибка парсинга файла");
    assert(DOM_tree.IsObject());
    // std::cout << "Parsing to document succeeded" << std::endl;
}

void XML_Parser::parse() {
    open_file();
    xml_to_json();
    parse_json_to_Document();
}

const Document& XML_Parser::get_DOM_tree() {
    return DOM_tree;
}
}  // namespace algoview_xml_parser

int main() {
    using namespace rapidjson;
    using namespace algoview_xml_parser;
    XML_Parser parser;
    parser.parse();
    const Document& tree = parser.get_DOM_tree();
}
