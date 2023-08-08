#include "json_traverser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "argument.hpp"
#include "block.hpp"
#include "expr.hpp"
#include "logger.hpp"
#include "vertex.hpp"

const std::string file_name = "json_traverse.cpp";

namespace algoview_json_traverser {

using namespace rapidjson;
using namespace reg_expr;
using namespace logger;

void parse_value(const std::string& str, std::string& left_value, std::string& right_value, char parse_sym) {
    const std::string func_name = "parse_value";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    std::string msg = "Start parsing string = " + str + " by symbol" + parse_sym;
    logger.log_info_msg(msg);

    int first_parse_sym_pos = str.find(parse_sym);
    if (first_parse_sym_pos == std::string::npos) {
        logger.log_err_msg(func_name, file_name, "Incorrect string or symbol");
        exit(1);
    }
    char* c_left_value = (char*)malloc(first_parse_sym_pos);
    if (c_left_value == NULL) {
        logger.log_err_msg(func_name, file_name, "Malloc error");
        exit(1);
    }
    int left_value_len = str.copy(c_left_value, first_parse_sym_pos, 0);
    if (left_value_len == 0) {
        logger.log_err_msg(func_name, file_name, "Copy error");
        exit(1);
    }
    c_left_value[left_value_len] = '\0';
    left_value = std::string(c_left_value);
    if (parse_sym == ',')
        first_parse_sym_pos++;
    else if (parse_sym == '.')
        first_parse_sym_pos += 2;
    int right_value_len = str.length() - first_parse_sym_pos;
    char* c_right_value = (char*)malloc(right_value_len);
    if (c_right_value == NULL) {
        logger.log_err_msg(func_name, file_name, "Malloc error");
        exit(1);
    }
    left_value_len = str.copy(c_right_value, right_value_len, first_parse_sym_pos);
    if (left_value_len == 0) {
        logger.log_err_msg(func_name, file_name, "Copy error");
        exit(1);
    }
    c_right_value[left_value_len] = '\0';
    right_value = std::string(c_right_value);

    logger.log_info_finish_msg("parsing string");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_arg_element(const Value& arg_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_arg_element";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);

    // assert(arg_tag.IsObject() && "Тег arg не является объектом");
    if (!arg_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Arg tag is not an Object");
        exit(1);
    }
    // assert(arg_tag.HasMember("@name") && "Не указано имя аргумента");
    if (!arg_tag.HasMember("@name")) {
        logger.log_err_msg(func_name, file_name, "Arg tag has no name");
        exit(1);
    }
    // assert(arg_tag["@name"].IsString());
    if (!arg_tag["@name"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Arg name is not a string");
        exit(1);
    }
    // printf("name = %s\n", arg_tag["@name"].GetString());
    logger.log_char_msg("Arg name = ", arg_tag["@name"].GetString());
    // assert(arg_tag.HasMember("@val") && "Не указано значение аргумента");
    if (!arg_tag.HasMember("@val")) {
        logger.log_err_msg(func_name, file_name, "Arg value range not specified");
        exit(1);
    }
    assert(arg_tag["@val"].IsString());
    // printf("val = %s\n", arg_tag["@val"].GetString());
    logger.log_char_msg("Arg val range = ", arg_tag["@val"].GetString());

    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    ArgTagsInfo& args = block.get_args();
    const ParamsMap& params = graph.get_params();

    std::string begin_expr, end_expr;
    parse_value(arg_tag["@val"].GetString(), begin_expr, end_expr, '.');
    // std::cout << "begin = " << begin_expr << "\tend = " << end_expr << std::endl;
    std::string msg = "After parsing : begin = " + begin_expr + "\tend = " + end_expr;
    logger.log_info_msg(msg);
    double begin = calc_expr(begin_expr, params);
    double end = calc_expr(end_expr, params);
    // std::cout << "begin = " << begin << "\tend = " << end << std::endl;
    msg.clear();
    msg = "After calculations : begin = " + std::to_string(begin) + "\tend = " + std::to_string(end);
    logger.log_info_msg(msg);
    args.add_arg(arg_tag["@name"].GetString(), (int)begin, (int)end);

    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_arg(const Value& arg_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_arg";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing args");

    if (arg_tag.IsArray()) {
        for (SizeType i = 0; i < arg_tag.Size(); i++) {
            traverse_arg_element(arg_tag[i], graph);
        }
    } else if (arg_tag.IsObject()) {
        traverse_arg_element(arg_tag, graph);
    } else {
        // assert("Тег arg имеет неправильный тип");
        logger.log_err_msg(func_name, file_name, "Arg tag has undefined type");
        exit(1);
    }

    logger.log_info_finish_msg("traversing args");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_vertex_in_element(const Value& in_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_vertex_in_element";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);

    // assert(in_tag.IsObject() && "Тег in не является объектом");
    if (!in_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "In tag is not an Object");
        exit(1);
    }
    // assert(in_tag.HasMember("@src") && "Не указан src");
    if (!in_tag.HasMember("@src")) {
        logger.log_err_msg(func_name, file_name, "Src attr not specified");
        exit(1);
    }
    // assert(in_tag["@src"].IsString());
    if (!in_tag["@src"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Src tag is not a string");
        exit(1);
    }
    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    VertexTagsInfo& vertices = block.get_vertices();
    if (in_tag.HasMember("@bsrc")) {
        // assert(in_tag["@bsrc"].IsString());
        if (!in_tag["@bsrc"].IsString()) {
            logger.log_err_msg(func_name, file_name, "Bsrc tag is not a string");
            exit(1);
        }
        logger.log_char_msg("bsrc block id = ", in_tag["@bsrc"].GetString());
        logger.log_char_msg("src coordinates = ", in_tag["@src"].GetString());
        vertices.add_bsrc(std::make_pair(std::stoi(in_tag["@bsrc"].GetString()), in_tag["@src"].GetString()));
        // printf("bsrc = %s src = %s\n", in_tag["@bsrc"].GetString(), in_tag["@src"].GetString());
    } else {
        logger.log_char_msg("src coordinates = ", in_tag["@src"].GetString());
        vertices.add_src(in_tag["@src"].GetString());
        // printf("src = %s\n", in_tag["@src"].GetString());
    }
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_vertex_in(const Value& in_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_vertex_in";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing sources");

    if (in_tag.IsArray()) {
        for (SizeType i = 0; i < in_tag.Size(); i++) {
            traverse_vertex_in_element(in_tag[i], graph);
        }
    } else if (in_tag.IsObject()) {
        traverse_vertex_in_element(in_tag, graph);
    } else {
        // assert("Тег in имеет неправильный тип");
        logger.log_err_msg(func_name, file_name, "In tag has undefined type");
        exit(1);
    }

    logger.log_info_finish_msg("traverse_vertex_in");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_vertex_element(const Value& vertex_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_vertex_element";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);

    // assert(vertex_tag.HasMember("@condition") && "Не указан condition");
    if (!vertex_tag.HasMember("@condition")) {
        logger.log_err_msg(func_name, file_name, "Vertex tag has no condition");
        exit(1);
    }
    // assert(vertex_tag["@condition"].IsString());
    if (!vertex_tag["@condition"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Condition is not a string");
        exit(1);
    }
    // assert(vertex_tag["@type"].IsString());
    if (!vertex_tag["@type"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Type is not a string");
        exit(1);
    }
    BlockTagsInfo& blocks = graph.get_blocks();
    BlockTagInfo& block = blocks.get_last_block();
    VertexTagsInfo& vertices = block.get_vertices();
    vertices.new_vertex();
    for (Value::ConstMemberIterator itr = vertex_tag.MemberBegin(); itr != vertex_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "@condition")) {
            logger.log_char_msg("Current vertex condition = ", vertex_tag["@condition"].GetString());
            vertices.add_condition(vertex_tag["@condition"].GetString());
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "@type")) {
            logger.log_char_msg("Current vertex type = ", vertex_tag["@type"].GetString());
            vertices.add_type(vertex_tag["@type"].GetString());
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "in")) {
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            logger.log_info_msg("Found first in tag");
            traverse_vertex_in(itr->value, graph);
        } else {
            // assert("Неопознанный тег внутри vertex");
            logger.log_err_msg(func_name, file_name, "Undefined tag inside tag vertex");
            exit(1);
        }
    }

    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_vertex(const Value& vertex_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_vertex";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing verticies");

    if (vertex_tag.IsArray()) {
        for (SizeType i = 0; i < vertex_tag.Size(); i++) {
            traverse_vertex_element(vertex_tag[i], graph);
        }
    } else if (vertex_tag.IsObject()) {
        traverse_vertex_element(vertex_tag, graph);
    } else {
        // assert("Тег vertex имеет неправильный тип");
        logger.log_err_msg(func_name, file_name, "Vertex tag has undefined type");
        exit(1);
    }

    logger.log_info_finish_msg("traversing verticies");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_block_element(const Value& block_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_block_element";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    // assert(block_tag.IsObject() && "Тег block не является объектом");
    if (!block_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Block tag is not an Object");
        exit(1);
    }
    if (block_tag.HasMember("id"))
        // assert(block_tag["@id"].IsString());
        if (!block_tag["@id"].IsString()) {
            logger.log_err_msg(func_name, file_name, "Id attr is not a string");
            exit(1);
        }
    // assert(block_tag.HasMember("@dims") && "Не указана размерность пространства");
    if (!block_tag.HasMember("@dims")) {
        logger.log_err_msg(func_name, file_name, "Dimensional not specified");
        exit(1);
    }
    // assert(block_tag["@dims"].IsString());
    if (!block_tag["@dims"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Dimensional is not a string");
        exit(1);
    }
    BlockTagsInfo& blocks = graph.get_blocks();
    blocks.new_block();
    for (Value::ConstMemberIterator itr = block_tag.MemberBegin(); itr != block_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "@id")) {
            // assert(block_tag["@id"].IsString());
            logger.log_char_msg("Start handling block with id = ", block_tag["@id"].GetString());
            blocks.add_id(std::stoi(block_tag["@id"].GetString()));
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "@dims")) {
            // assert(block_tag["@dims"].IsString());
            if (!block_tag["@dims"].IsString()) {
                exit(1);
            }
            logger.log_char_msg("Current block dim = ", block_tag["@dims"].GetString());
            blocks.add_dim(std::stoi(block_tag["@dims"].GetString()));
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        } else if (!strcmp(itr->name.GetString(), "arg")) {
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            logger.log_info_msg("Found first tag arg");
            traverse_arg(itr->value, graph);
        } else if (!strcmp(itr->name.GetString(), "vertex")) {
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            logger.log_info_msg("Found first tag vertex");
            traverse_vertex(itr->value, graph);
        } else {
            // assert("Неопознанный тег внутри block");
            logger.log_err_msg(func_name, file_name, "Undefined tag inside block tag");
            exit(1);
        }
    }
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_block(const Value& block_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_block";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing blocks");

    if (block_tag.IsArray()) {
        for (SizeType i = 0; i < block_tag.Size(); i++) {
            traverse_block_element(block_tag[i], graph);
        }
    } else if (block_tag.IsObject()) {
        traverse_block_element(block_tag, graph);
    } else {
        // assert("Тег block имеет неправильный тип");
        logger.log_err_msg(func_name, file_name, "Block tag has undefined type");
        exit(1);
    }

    logger.log_info_finish_msg("traversing blocks");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_param_element(const Value& param_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_param_element";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("handling parameter");

    // assert(param_tag.IsObject() && "Тег param не является объектом");
    if (!param_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Param tag is not an Object");
        exit(1);
    }
    // assert(param_tag.HasMember("@name") && "Не указано имя параметра");
    // *for user
    if (!param_tag.HasMember("@name")) {
        logger.log_err_msg(func_name, file_name, "Parameter has no name");
        exit(1);
    }
    // assert(param_tag["@name"].IsString());
    // *for user
    if (!param_tag["@name"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Parameter's name is not a string");
        exit(1);
    }
    // printf("name = %s\n", param_tag["@name"].GetString());
    // assert(param_tag.HasMember("@type") && "Не указан тип параметра");
    // *for user
    if (!param_tag.HasMember("@type")) {
        logger.log_err_msg(func_name, file_name, "Parameter has no type");
        exit(1);
    }
    // assert(param_tag["@type"].IsString());
    // *for user
    if (!param_tag["@type"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Parameter's type is not a string");
        exit(1);
    }
    // printf("type = %s\n", param_tag["@type"].GetString());

    // if (param_tag.HasMember("@value")) {
    //     assert(param_tag["@value"].IsString());
    //     printf("val = %s\n", param_tag["@value"].GetString());
    // }

    // assert(param_tag.HasMember("@value"));
    //*for user
    if (!param_tag.HasMember("@value")) {
        logger.log_err_msg(func_name, file_name, "Parameter has no value");
        exit(1);
    }
    // assert(param_tag["@value"].IsString());
    if (!param_tag["@value"].IsString()) {
        logger.log_err_msg(func_name, file_name, "Parameter's value is not a string");
        exit(1);
    }
    // printf("val = %s\n", param_tag["@value"].GetString());
    graph.add_param(param_tag["@name"].GetString(), std::stoi(param_tag["@value"].GetString()));

    logger.log_info_finish_msg("handling parameter");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_param(const Value& param_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_param";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);

    if (param_tag.IsArray()) {
        for (SizeType i = 0; i < param_tag.Size(); i++) {
            traverse_param_element(param_tag[i], graph);
        }
    } else if (param_tag.IsObject()) {
        traverse_param_element(param_tag, graph);
    } else {
        // assert("Тег param имеет неправильный тип");
        logger.log_err_msg(func_name, file_name, "Param tag has undefined type");
        exit(1);
    }

    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_params(const Value& params_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_params";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing parameters");

    // assert(params_tag.IsObject() && "Тег params не является объектом");
    if (!params_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Params tag is not an Object");
        exit(1);
    }
    // assert(params_tag.HasMember("param") && "Не указаны параметры");
    // *for user
    if (!params_tag.HasMember("param")) {
        logger.log_err_msg(func_name, file_name, "Params tag does not contain parameter's definitions");
        exit(1);
    }
    traverse_param(params_tag["param"], graph);

    logger.log_info_finish_msg("traversing parameters");
    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse_algo(const Value& algo_tag, GraphInfo& graph) {
    const std::string func_name = "traverse_algo";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);

    logger.log_info_start_msg("traversing tags inside algo tag");
    for (Value::ConstMemberIterator itr = algo_tag.MemberBegin(); itr != algo_tag.MemberEnd(); ++itr) {
        if (!strcmp(itr->name.GetString(), "params")) {
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            logger.log_info_msg("Found tag params");
            traverse_params(itr->value, graph);
        } else if (!strcmp(itr->name.GetString(), "block")) {
            // printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            logger.log_info_msg("Found tag block");
            traverse_block(itr->value, graph);
        } else {
            // assert("Неопознанный тег внутри algo");
            // *for user
            logger.log_err_msg(func_name, file_name, "Undefined tag inside algo tag");
            exit(1);
        }
    }
    logger.log_info_finish_msg("traversing tags inside algo tag");

    logger.log_file_exit(func_name, file_name);
}

void JSON_Traverser::traverse(const Document& doc, GraphInfo& graph) {
    // void JSON_Traverser::traverse(const Document& doc) {
    const std::string func_name = "traverse";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("traversing DOM tree");

    const Value& head_tag = doc["algo"];
    // assert(head_tag.IsObject() && "Тег algo не является объектом");
    if (!head_tag.IsObject()) {
        logger.log_err_msg(func_name, file_name, "Algo tag is not an Object");
        exit(1);
    }
    traverse_algo(head_tag, graph);

    logger.log_info_finish_msg("traversing DOM tree");
    logger.log_file_exit(func_name, file_name);
}

}  // namespace algoview_json_traverser
