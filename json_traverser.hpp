#pragma once
#include "graph_info.hpp"
#include "include/rapidjson/pointer.h"
#include "include/rapidjson/prettywriter.h"

namespace algoview_json_traverser {

using namespace rapidjson;
using namespace graph_info;

static const char* kTypeNames[] = {"Null", "False", "True", "Object", "Array", "String", "Number"};

class JSON_Traverser {
   public:
    static void traverse_arg_element(const Value& arg_tag, GraphInfo& graph);
    static void traverse_arg(const Value& arg_tag, GraphInfo& graph);
    static void traverse_vertex_in_element(const Value& in_tag, GraphInfo& graph);
    static void traverse_vertex_in(const Value& in_tag, GraphInfo& graph);
    static void traverse_vertex_element(const Value& vertex_tag, GraphInfo& graph);
    static void traverse_vertex(const Value& vertex_tag, GraphInfo& graph);
    static void traverse_block_element(const Value& block_tag, GraphInfo& graph);
    static void traverse_block(const Value& block_tag, GraphInfo& graph);
    static void traverse_param_element(const Value& param_tag, GraphInfo& graph);
    static void traverse_param(const Value& param_tag, GraphInfo& graph);
    static void traverse_params(const Value& param_tag, GraphInfo& graph);
    static void traverse_algo(const Value& algo_tag, GraphInfo& graph);
    static void traverse(const Document& doc, GraphInfo& graph);

   private:
};

void parse_value(const std::string& str, std::string& left_value, std::string& right_value, char parse_sym);

}  // namespace algoview_json_traverser