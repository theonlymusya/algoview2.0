#include "json_traverser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "argument.hpp"
#include "block.hpp"
#include "expr.hpp"
#include "vertex.hpp"

namespace algoview_json_traverser {

using namespace rapidjson;
using namespace reg_expr;

void parse_value(const std::string& str, std::string& left_value, std::string& right_value, char parse_sym) {
    int first_parse_sym_pos = str.find(parse_sym);
    char* c_left_value = (char*)malloc(first_parse_sym_pos);
    int left_value_len = str.copy(c_left_value, first_parse_sym_pos, 0);
    c_left_value[left_value_len] = '\0';
    left_value = std::string(c_left_value);
    if (parse_sym == ',')
        first_parse_sym_pos++;
    else if (parse_sym == '.')
        first_parse_sym_pos += 2;
    int right_value_len = str.length() - first_parse_sym_pos;
    char* c_right_value = (char*)malloc(right_value_len);
    left_value_len = str.copy(c_right_value, right_value_len, first_parse_sym_pos);
    c_right_value[left_value_len] = '\0';
    right_value = std::string(c_right_value);
}

void JSON_Traverser::traverse_arg_element(const Value& arg_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_arg_element(const Value& arg_tag) {
    assert(arg_tag.IsObject() && "Тег arg не является объектом");
    assert(arg_tag.HasMember("@name") && "Не указано имя аргумента");
    assert(arg_tag["@name"].IsString());
    printf("name = %s\n", arg_tag["@name"].GetString());
    assert(arg_tag.HasMember("@val") && "Не указано значение аргумента");
    assert(arg_tag["@val"].IsString());
    printf("val = %s\n", arg_tag["@val"].GetString());

    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    ArgTagsInfo& args = block.get_args();
    const ParamsMap& params = graph.get_params();

    std::string begin_expr, end_expr;
    parse_value(arg_tag["@val"].GetString(), begin_expr, end_expr, '.');
    std::cout << "begin = " << begin_expr << "\tend = " << end_expr << std::endl;
    double begin = calc_expr(begin_expr, params);
    double end = calc_expr(end_expr, params);
    std::cout << "begin = " << begin << "\tend = " << end << std::endl;
    args.add_arg(arg_tag["@name"].GetString(), (int)begin, (int)end);
}

void JSON_Traverser::traverse_arg(const Value& arg_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_arg(const Value& arg_tag) {
    if (arg_tag.IsArray()) {
        for (SizeType i = 0; i < arg_tag.Size(); i++) {
            traverse_arg_element(arg_tag[i], graph);
            // traverse_arg_element(arg_tag[i]);
        }
    } else if (arg_tag.IsObject()) {
        traverse_arg_element(arg_tag, graph);
        // traverse_arg_element(arg_tag);
    } else {
        assert("Тег arg имеет неправильный тип");
    }
}

void JSON_Traverser::traverse_vertex_in_element(const Value& in_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_vertex_in_element(const Value& in_tag) {
    assert(in_tag.IsObject() && "Тег in не является объектом");
    assert(in_tag.HasMember("@src") && "Не указан src");
    assert(in_tag["@src"].IsString());
    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    VertexTagsInfo& vertices = block.get_vertices();
    if (in_tag.HasMember("@bsrc")) {
        assert(in_tag["@bsrc"].IsString());
        vertices.add_bsrc(std::make_pair(in_tag["@bsrc"].GetString(), in_tag["@src"].GetString()));
        printf("bsrc = %s src = %s\n", in_tag["@bsrc"].GetString(), in_tag["@src"].GetString());
    } else {
        vertices.add_src(in_tag["@src"].GetString());
        printf("src = %s\n", in_tag["@src"].GetString());
    }
}

void JSON_Traverser::traverse_vertex_in(const Value& in_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_vertex_in(const Value& in_tag) {
    if (in_tag.IsArray()) {
        for (SizeType i = 0; i < in_tag.Size(); i++) {
            traverse_vertex_in_element(in_tag[i], graph);
            // traverse_vertex_in_element(in_tag[i]);
        }
    } else if (in_tag.IsObject()) {
        traverse_vertex_in_element(in_tag, graph);
        // traverse_vertex_in_element(in_tag);
    } else {
        assert("Тег in имеет неправильный тип");
    }
}

void JSON_Traverser::traverse_vertex_element(const Value& vertex_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_vertex_element(const Value& vertex_tag) {
    assert(vertex_tag.HasMember("@condition") && "Не указан condition");
    assert(vertex_tag["@condition"].IsString());
    assert(vertex_tag["@type"].IsString());
    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    VertexTagsInfo& vertices = block.get_vertices();
    vertices.new_vertex();
    for (Value::ConstMemberIterator itr = vertex_tag.MemberBegin(); itr != vertex_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "@condition")) {
            assert(vertex_tag["@condition"].IsString());
            vertices.add_condition(vertex_tag["@condition"].GetString());
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "@type")) {
            assert(vertex_tag["@type"].IsString());
            vertices.add_type(vertex_tag["@type"].GetString());
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "in")) {
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            traverse_vertex_in(itr->value, graph);
            // traverse_vertex_in(itr->value);
        } else {
            assert("Неопознанный тег внутри vertex");
        }
    }
}

void JSON_Traverser::traverse_vertex(const Value& vertex_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_vertex(const Value& vertex_tag) {
    if (vertex_tag.IsArray()) {
        for (SizeType i = 0; i < vertex_tag.Size(); i++) {
            traverse_vertex_element(vertex_tag[i], graph);
            // traverse_vertex_element(vertex_tag[i]);
        }
    } else if (vertex_tag.IsObject()) {
        traverse_vertex_element(vertex_tag, graph);
        // traverse_vertex_element(vertex_tag);
    } else {
        assert("Тег vertex имеет неправильный тип");
    }
}

void JSON_Traverser::traverse_block_element(const Value& block_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_block_element(const Value& block_tag) {
    assert(block_tag.IsObject() && "Тег block не является объектом");
    if (block_tag.HasMember("id"))
        assert(block_tag["@id"].IsString());
    assert(block_tag.HasMember("@dims") && "Не указана размерность пространства");
    assert(block_tag["@dims"].IsString());
    BlockTagsInfo& blocks = graph.get_blocks();
    blocks.new_block();
    for (Value::ConstMemberIterator itr = block_tag.MemberBegin(); itr != block_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "@id")) {
            assert(block_tag["@id"].IsString());
            blocks.add_id(std::stoi(block_tag["@id"].GetString()));
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "@dims")) {
            assert(block_tag["@dims"].IsString());
            blocks.add_dim(std::stoi(block_tag["@dims"].GetString()));
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "arg")) {
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            traverse_arg(itr->value, graph);
            // traverse_arg(itr->value);
        } else if (!strcmp(itr->name.GetString(), "vertex")) {
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            traverse_vertex(itr->value, graph);
            // traverse_vertex(itr->value);
        } else {
            assert("Неопознанный тег внутри block");
        }
    }
}

void JSON_Traverser::traverse_block(const Value& block_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_block(const Value& block_tag) {
    if (block_tag.IsArray()) {
        for (SizeType i = 0; i < block_tag.Size(); i++) {
            traverse_block_element(block_tag[i], graph);
            // traverse_block_element(block_tag[i]);
        }
    } else if (block_tag.IsObject()) {
        traverse_block_element(block_tag, graph);
        // traverse_block_element(block_tag);
    } else {
        assert("Тег block имеет неправильный тип");
    }
}

void JSON_Traverser::traverse_param_element(const Value& param_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_param_element(const Value& param_tag) {
    assert(param_tag.IsObject() && "Тег param не является объектом");
    assert(param_tag.HasMember("@name") && "Не указано имя параметра");
    assert(param_tag["@name"].IsString());
    printf("name = %s\n", param_tag["@name"].GetString());
    assert(param_tag.HasMember("@type") && "Не указан тип параметра");
    assert(param_tag["@type"].IsString());
    printf("type = %s\n", param_tag["@type"].GetString());
    // if (param_tag.HasMember("@value")) {
    //     assert(param_tag["@value"].IsString());
    //     printf("val = %s\n", param_tag["@value"].GetString());
    // }
    assert(param_tag.HasMember("@value"));
    assert(param_tag["@value"].IsString());
    printf("val = %s\n", param_tag["@value"].GetString());
    graph.add_param(param_tag["@name"].GetString(), std::stoi(param_tag["@value"].GetString()));
}

void JSON_Traverser::traverse_param(const Value& param_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_param(const Value& param_tag) {
    if (param_tag.IsArray()) {
        for (SizeType i = 0; i < param_tag.Size(); i++) {
            traverse_param_element(param_tag[i], graph);
            // traverse_param_element(param_tag[i]);
        }
    } else if (param_tag.IsObject()) {
        traverse_param_element(param_tag, graph);
        // traverse_param_element(param_tag);
    } else {
        assert("Тег param имеет неправильный тип");
    }
}

void JSON_Traverser::traverse_params(const Value& params_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_params(const Value& params_tag) {
    assert(params_tag.IsObject() && "Тег params не является объектом");
    assert(params_tag.HasMember("param") && "Не указаны параметры");
    traverse_param(params_tag["param"], graph);
    // traverse_param(params_tag["param"]);
}

void JSON_Traverser::traverse_algo(const Value& algo_tag, GraphInfo& graph) {
    // void JSON_Traverser::traverse_algo(const Value& algo_tag) {
    for (Value::ConstMemberIterator itr = algo_tag.MemberBegin(); itr != algo_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "params")) {
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            traverse_params(itr->value, graph);
            // traverse_params(itr->value);
        } else if (!strcmp(itr->name.GetString(), "block")) {
            printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            traverse_block(itr->value, graph);
            // traverse_block(itr->value);
        } else {
            assert("Неопознанный тег внутри algo");
        }
    }
}

void JSON_Traverser::traverse(const Document& doc, GraphInfo& graph) {
    // void JSON_Traverser::traverse(const Document& doc) {
    const Value& head_tag = doc["algo"];
    assert(head_tag.IsObject() && "Тег algo не является объектом");
    traverse_algo(head_tag, graph);
    // traverse_algo(head_tag);
}

}  // namespace algoview_json_traverser
