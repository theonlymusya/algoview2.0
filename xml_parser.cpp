#include "xml_parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "include/rapidjson/document.h"
#include "include/xml2json.hpp"

namespace algoview_xml_parser {

using namespace rapidjson;

void XML_Parser::open_file(const std::string& file_name) {
    file_.open(file_name);
}

void XML_Parser::xml_to_json() {
    std::ostringstream oss;
    oss << file_.rdbuf();
    json_str_ = xml2json(oss.str().data());
}

void XML_Parser::print_json() {
    std::cout << json_str_ << std::endl;
}

void XML_Parser::parse_json_to_Document() {
    std::cout << "Ready to parse json" << std::endl;
    const char* json_cstr = json_str_.c_str();
    assert(!DOM_tree_.Parse(json_cstr).HasParseError() && "Ошибка парсинга файла");
    assert(DOM_tree_.IsObject());
    std::cout << "Parsing to document succeeded" << std::endl;
}

void XML_Parser::parse(const std::string& file_name) {
    open_file(file_name);
    xml_to_json();
    parse_json_to_Document();
}

const Document& XML_Parser::get_DOM_tree() {
    return DOM_tree_;
}
}  // namespace algoview_xml_parser
