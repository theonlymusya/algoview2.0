#include "xml_parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "include/rapidjson/document.h"
#include "include/xml2json.hpp"
#include "logger.hpp"

const std::string file_name = "xml_parser.cpp";

namespace algoview_xml_parser {

using namespace rapidjson;
using namespace logger;

void XML_Parser::open_file(const std::string& filename) {
    const std::string func_name = "open_file";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_msg("Start opening xml file");

    file_.open(filename);
    if (file_.fail()) {
        logger.log_err_msg(func_name, file_name, strerror(errno));
        exit(1);
    }

    logger.log_info_msg("Finish opening xml file successfully");
    logger.log_file_exit(func_name, file_name);
}

void XML_Parser::xml_to_json() {
    const std::string func_name = "open_file";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_msg("Start converting xml to json");

    std::ostringstream oss;
    oss << file_.rdbuf();
    json_str_ = xml2json(oss.str().data());

    logger.log_info_msg("Finish converting xml to json successfully");
    logger.log_file_exit(func_name, file_name);
}

void XML_Parser::print_json() {
    std::cout << json_str_ << std::endl;
}

void XML_Parser::parse_json_to_Document() {
    const std::string func_name = "parse_json_to_Document";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_msg("Start parsing json file to Document object");

    const char* json_cstr = json_str_.c_str();
    // assert(!DOM_tree_.Parse(json_cstr).HasParseError() && "Ошибка парсинга файла");
    if (DOM_tree_.Parse(json_cstr).HasParseError()) {
        logger.log_err_msg(func_name, file_name, "XML parse error");
        exit(1);
    }
    // assert(DOM_tree_.IsObject());
    if (!DOM_tree_.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Document is not an Object (internal parse error)");
        exit(1);
    }
    logger.log_info_msg("Finish parsing json file to Document object successfully");
    logger.log_file_exit(func_name, file_name);
}

void XML_Parser::parse(const std::string& filename) {
    open_file(filename);
    xml_to_json();
    file_.close();
    parse_json_to_Document();
}

const Document& XML_Parser::get_DOM_tree() {
    return DOM_tree_;
}
}  // namespace algoview_xml_parser
