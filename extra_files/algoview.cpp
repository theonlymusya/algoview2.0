#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "exprtk.hpp"
#include "include/rapidjson/document.h"
#include "include/rapidjson/pointer.h"
#include "include/rapidjson/prettywriter.h"
#include "include/xml2json.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

const bool debug_mode = true;

constexpr int X = 0;
constexpr int Y = 1;
constexpr int Z = 2;

using CoordType = double;
using EdgeId = int;
using EdgeType = int;
using VertexId = int;
using VertexType = std::string;
using BlockId = std::string;
using Dim = int;
using namespace rapidjson;

template <typename T>
const T calc_expr(std::string expr, const std::map<std::string, T>& variables_values_map) {
    typedef exprtk::symbol_table<T> symbol_table_t;
    typedef exprtk::expression<T> expression_t;
    typedef exprtk::parser<T> parser_t;

    const std::string expression_string(expr);
    std::vector<T> variables(variables_values_map.size());

    symbol_table_t symbol_table;

    int var_ind = 0;
    for (auto it = variables_values_map.cbegin(); it != variables_values_map.cend(); ++it) {
        if (expr.find(it->first) != std::string::npos) {
            symbol_table.add_variable(it->first, variables[var_ind]);
            variables[var_ind] = it->second;
            var_ind++;
        }
    }

    symbol_table.add_constants();

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    parser.compile(expression_string, expression);

    return expression.value();
}

struct VertexTagInfo {
    VertexType type = "1";
    std::string cond = " ";
    std::vector<std::string> src;
    std::vector<std::string> bsrc;
};

struct ArgTagInfo {
    std::string name;
    int begin;
    int end;
};

class ArgsTagInfo {
   public:
    ArgsTagInfo() {
        for (int i = 0; i < 3; i++) {
            arg_tags.push_back(ArgTagInfo{"_", 0, 0});
        }
    }

    void add_arg(std::string name, int begin_value, int end_value) {
        assert((n < 3) && "3D graph only\n");
        std::cout << "in ArgsTagInfo::add_arg\n";
        arg_tags[n] = ArgTagInfo{name, begin_value, end_value};
        std::cout << "2\n";
        n++;
    }

    std::vector<ArgTagInfo> arg_tags;

   private:
    int n = 0;
};

class DOM_tree {
   public:
    void parse_xml2json(std::string);
    void parse_xml2tree(std::string);
    void print_tree();
    void print_json();
    // find out root node
    rapidxml::xml_node<>* get_root_node() { return doc_tree_.first_node("algo"); };
    void print_params_map();
    void print_blocks();
    void json_traverse();
    void json_traverse_rec(const Value&);
    // void handle_tree(DOM_tree&);
    // void handle_params_tag();
    const std::map<std::string, CoordType>& get_param_value_map() const { return param_value_map_; }
    class Block {
       public:
        BlockId id = 0;
        Dim dim;
        Block(const BlockId& _id, Dim _dim) : id(_id), dim(_dim) {}
        void handle_arg_tag(DOM_tree&, Block&, const rapidxml::xml_node<>*);
        void handle_vertex_tag(DOM_tree&, Block&, const rapidxml::xml_node<>*);
        void print_args() const;
        void print_vertices() const;
        // void print_vars_map();
        // const std::map<std::string, CoordType>& get_var_range_map() const {
        //     return var_range_map_;
        // }
        // void handle_vertex_cond();
       private:
        ArgsTagInfo args = ArgsTagInfo();
        std::vector<VertexTagInfo> vertices;
        // std::map<std::string, CoordType> var_range_map_;
        // std::vector<std::map<std::string, CoordType>> vertex_cond_vec_;
    };
    Block handle_block_tag(DOM_tree&, const rapidxml::xml_node<>*);
    const std::vector<Block>& get_blocks() const { return blocks_; }

   private:
    rapidxml::xml_document<> doc_tree_;  // character type defaults to char
    Document doc_json_;
    std::string json_str_;
    std::map<std::string, CoordType> param_value_map_;
    std::vector<Block> blocks_;
};

class Graph {
   public:
    void vertices_init(const std::vector<DOM_tree::Block>&);
    void vertices_coords_calc(const std::vector<DOM_tree::Block>&);
    void print_vertices();
    VertexId get_vertex_id() { return vertex_id_++; }
    class Vertex {
       public:
        VertexId id;
        VertexType type;
        std::vector<CoordType> coords;
        Vertex(VertexId _id, VertexType _type = "0") : id(_id), type(_type) { coords.resize(3, 0); }
    };
    void add_vertex(int);
    void offset_coords(int, int, int, int);
    std::string to_json_format();

   private:
    int offset = 1;
    std::vector<Vertex> vertices_;
    VertexId vertex_id_ = 0;
};

// void Graph::add_vertex(int size) {
//     for (int i = 0; i < size; i++) {
//         vertices_.emplace_back(Vertex(get_vertex_id()));
//     }
// }

// void Graph::vertices_init(const std::vector<DOM_tree::Block>& blocks) {
//     for (const auto& block : blocks) {
//         std::cerr << "ya block";
//         const auto& vars_range_map = block.get_var_range_map();
//         if (block.dim == 1) {
//             add_vertex(vars_range_map.find("i")->second);
//         }
//         if (block.dim == 2) {
//             std::cerr << "ya block dim 2" << std::endl;
//             std::cerr << vars_range_map.find("i")->second *
//                              vars_range_map.find("j")->second
//                       << std::endl;
//             add_vertex(vars_range_map.find("i")->second *
//                        vars_range_map.find("j")->second);
//         }
//         if (block.dim == 3) {
//             std::cerr << "ya block dim 3" << std::endl;
//             std::cerr << vars_range_map.find("i")->second *
//                              vars_range_map.find("j")->second *
//                              vars_range_map.find("k")->second
//                       << std::endl;
//             add_vertex(vars_range_map.find("i")->second *
//                        vars_range_map.find("j")->second *
//                        vars_range_map.find("k")->second);
//         }
//     }
//     std::cerr << "ecgt[";
// }

// void Graph::offset_coords(int local_offset, int size, int coord, int
// vert_num) {
//     // void Graph::offset_coords(int local_offset, int cycle_offset, int
//     coord,
//     // int vert_num) {
//     std::cout << "local_offset: " << local_offset << std::endl;
//     std::cout << "size: " << size << std::endl;
//     std::cout << "coord: " << coord << std::endl;
//     std::cout << "vert_num: " << vert_num << std::endl;
//     for (int i = 0; i < size - 1; i++) {
//         for (int j = 0; j < size - i - 1; j++) {
//             for (int k = vert_num - j - 1; k >= 0; k -= size) {
//                 vertices_[local_offset + k].coords[coord] += offset;
//             }
//         }
//     }
//     // int i = 0;
//     // while (i < vert_num) {
//     // 	i += cycle_offset;
//     // 	for (int j = 0; j < cycle_offset; j++) {
//     // 		vertices_[local_offset + i + j].coords[coord] += offset;
//     // 	}
//     // 	i+= cycle_offset;
//     // 	if (dim == 3) {
//     // 		for (int j = 0; j < cycle_offset; j++) {
//     // 			vertices_[local_offset + i + j].coords[coord] +=
//     // 2*offset;
//     // 		}
//     // 		i+= cycle_offset;
//     // 	}
//     // }
// }

// void Graph::vertices_coords_calc(const std::vector<DOM_tree::Block>& blocks)
// {
//     vertices_init(blocks);
//     // смещение всех вершин относительно начала координат
//     for (auto& vertex : vertices_) {
//         vertex.coords[X] += offset;
//         vertex.coords[Y] += offset;
//         vertex.coords[Z] += offset;
//     }
//     print_vertices();
//     int vertices_arr_offset = 0;
//     int between_blocks_offset = offset;
//     for (const auto& block : blocks) {
//         const auto& vars_range_map = block.get_var_range_map();
//         int vertices_in_block_num;
//         if (block.dim == 1) {
//             vertices_in_block_num = vars_range_map.find("i")->second;
//         }
//         if (block.dim == 2) {
//             vertices_in_block_num = vars_range_map.find("i")->second *
//                                     vars_range_map.find("j")->second;
//         }
//         if (block.dim == 3) {
//             vertices_in_block_num = vars_range_map.find("i")->second *
//                                     vars_range_map.find("j")->second *
//                                     vars_range_map.find("k")->second;
//         }
//         if (block.dim == 1 || block.dim == 2 || block.dim == 3) {
//             offset_coords(vertices_arr_offset,
//             vars_range_map.find("i")->second,
//                           X, vertices_in_block_num);
//             between_blocks_offset = offset;
//         }
//         std::cerr << "--------------------------------------------------"
//                   << std::endl;
//         print_vertices();
//         if (block.dim == 2 || block.dim == 3) {
//             offset_coords(vertices_arr_offset,
//             vars_range_map.find("j")->second,
//                           Y, vertices_in_block_num);
//             between_blocks_offset = offset;
//         }
//         std::cerr << "--------------------------------------------------"
//                   << std::endl;
//         print_vertices();
//         if (block.dim == 3) {
//             offset_coords(vertices_arr_offset,
//             vars_range_map.find("k")->second,
//                           Z, vertices_in_block_num);
//             between_blocks_offset = vars_range_map.find("k")->second *
//             offset;
//         }
//         std::cerr << "--------------------------------------------------"
//                   << std::endl;
//         print_vertices();
//         vertices_arr_offset = vertices_in_block_num;
//         for (int i = vertices_arr_offset; i < vertices_.size(); i++) {
//             vertices_[i].coords[Z] += between_blocks_offset;
//         }
//     }
// }

std::string Graph::to_json_format() {
    std::string result_string;
    result_string += "{\n\t\"vertices\": [\n";
    for (const auto& vertex : vertices_) {
        std::string vertex_string = "\t\t{ \"id\": " + std::to_string(vertex.id) + ", \"coordinates\": [" +
                                    std::to_string((int)vertex.coords[X]) + ", " +
                                    std::to_string((int)vertex.coords[Y]) + ", " +
                                    std::to_string((int)vertex.coords[Z]) + "], \"type\": \"" + vertex.type + "\" },\n";
        result_string += vertex_string;
    }
    result_string.pop_back();
    result_string += "\t]\n}\n";
    return result_string;
}

// void Graph::print_vertices() {
//     for (const auto& vertex : vertices_) {
//         std::cout << vertex.id << ". [" << vertex.coords[X] << ", "
//                   << vertex.coords[Y] << ", " << vertex.coords[Z] << "]"
//                   << std::endl;
//     }
// }

void DOM_tree::parse_xml2tree(std::string filename) {
    std::cout << "Start parsing a test file to tree" << std::endl;
    std::ifstream file(filename);  // open file by using constructor
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc_tree_.parse<0>(&buffer[0]);  // <0> means default parse flags
    printf("\nParsing to document succeeded.\n");
}

void DOM_tree::parse_xml2json(std::string filename) {
    std::cout << "Start parsing a test file to json" << std::endl;
    std::ifstream file(filename);  // open file by using constructor
    std::ostringstream oss;
    oss << file.rdbuf();
    json_str_ = xml2json(oss.str().data());
    // std::cout << json_str_ << std::endl;
    const char* json_cstr = json_str_.c_str();
    assert(!doc_json_.Parse(json_cstr).HasParseError());
    printf("\nParsing to document succeeded.\n");
}

void DOM_tree::print_tree() {
    // print DOM tree by using internal tools of rapidxml
    std::cout << "------------By using internal tools------------" << std::endl;
    std::cout << doc_tree_;
}

void DOM_tree::print_json() {
    std::cout << json_str_ << std::endl;
}

void DOM_tree::handle_params_tag() {
    // get parameters names and values
    rapidxml::xml_node<>* params_node = get_root_node()->first_node("params");
    for (rapidxml::xml_node<>* param_node = params_node->first_node("param"); param_node;
         param_node = param_node->next_sibling()) {
        param_value_map_[param_node->first_attribute("name")->value()] =
            std::stoi(param_node->first_attribute("value")->value());
    }
}

void DOM_tree::print_params_map() {
    for (auto it = param_value_map_.cbegin(); it != param_value_map_.cend(); ++it)
        std::cout << it->first << " " << it->second << std::endl;
}

// void DOM_tree::Block::print_vars_map() {
//     for (auto it = var_range_map_.cbegin(); it != var_range_map_.cend();
//     ++it)
//         std::cout << it->first << " " << it->second << std::endl;
// }

void DOM_tree::Block::print_args() const {
    for (const auto& arg : args.arg_tags) {
        std::cout << arg.name << " : " << arg.begin << ".." << arg.end << std::endl;
    }
}

void DOM_tree::Block::print_vertices() const {
    for (const auto& vertex : vertices) {
        std::cout << "type : " << vertex.type << " condition : " << vertex.cond << std::endl;
        std::cout << "src : ";
        for (const auto& src : vertex.src) {
            std::cout << src << " ";
        }
        std::cout << std::endl << "bsrc : ";
        for (const auto& bsrc : vertex.bsrc) {
            std::cout << bsrc << " ";
        }
    }
}

void DOM_tree::print_blocks() {
    for (const auto& block : blocks_) {
        std::cout << "id: " << block.id << std::endl;
        std::cout << "dim: " << block.dim << std::endl;
        // const std::map<std::string, CoordType>& var_range_map =
        //     block.get_var_range_map();
        // for (auto it = var_range_map.cbegin(); it != var_range_map.cend();
        // ++it)
        //     std::cout << it->first << " " << it->second << std::endl;
        block.print_args();
        block.print_vertices();
    }
}

void DOM_tree::Block::handle_arg_tag(DOM_tree& tree, Block& block, const rapidxml::xml_node<>* arg_node) {
    printf("in DOM_tree::Block::handle_arg_tag\n");

    std::cout << arg_node ? 1 : 0;

    std::string var_name = arg_node->first_attribute("name")->value();
    std::string var_range = arg_node->first_attribute("val")->value();

    std::cerr << var_range << std::endl;
    std::cerr << var_name << std::endl;

    int first_dot_pos = var_range.find('.');
    char* c_begin_value = (char*)malloc(first_dot_pos);
    int len = var_range.copy(c_begin_value, first_dot_pos, 0);
    c_begin_value[len] = '\0';
    first_dot_pos += 2;
    char* c_end_value = (char*)malloc(var_range.length() - first_dot_pos);
    len = var_range.copy(c_end_value, var_range.length() - first_dot_pos, first_dot_pos);
    c_end_value[len] = '\0';

    std::cout << std::string(c_begin_value) << " ";
    std::cout << std::string(c_end_value) << " ";
    const std::map<std::string, CoordType>& param_value_map = tree.get_param_value_map();
    double expr_value_right = calc_expr<double>(std::string(c_end_value), param_value_map);
    double expr_value_left = calc_expr<double>(std::string(c_begin_value), param_value_map);
    printf("left barrier %8.2f ", expr_value_left);
    printf("right barrier %8.2f ", expr_value_right);
    std::cout << "var_range " << expr_value_right - expr_value_left << std::endl;
    std::cout << "1\n";

    block.args.add_arg(var_name, (int)expr_value_left, (int)expr_value_right);
    // block.var_range_map_[var_name] =
    //     (int)(expr_value - std::stoi(std::string(c_begin_value)) + 1);

    std::cout << "3\n";
}

void DOM_tree::Block::handle_vertex_tag(DOM_tree& tree, Block& block, const rapidxml::xml_node<>* vertex_node) {
    printf("in DOM_tree::Block::handle_vertex_tag\n");
    VertexTagInfo vertex;
    std::string condition = vertex_node->first_attribute("condition")->value();
    std::string type = vertex_node->first_attribute("type")->value();
    vertex.type = type;
    vertex.cond = condition;
    std::cout << condition;
    for (rapidxml::xml_node<>* in_node = vertex_node->first_node("in"); in_node; in_node = in_node->next_sibling()) {
        if (in_node->first_attribute()->name() == "src") {
            vertex.src.push_back(in_node->first_attribute("src")->value());
        } else if (in_node->first_attribute()->name() == "bsrc") {
            vertex.bsrc.push_back(in_node->first_attribute("bsrc")->value());
        }
    }
    vertices.push_back(vertex);
}

DOM_tree::Block DOM_tree::handle_block_tag(DOM_tree& tree, const rapidxml::xml_node<>* block_node) {
    std::string block_id = block_node->first_attribute("id")->value();
    std::string block_dim = block_node->first_attribute("dims")->value();
    DOM_tree::Block block(block_id, std::stoi(block_dim));
    rapidxml::xml_node<>* args_node = block_node->first_node("args");
    std::cout << "0\n";
    for (rapidxml::xml_node<>* arg_node = args_node->first_node("arg"); arg_node; arg_node = arg_node->next_sibling()) {
        // std::cout << arg_node.;
        block.handle_arg_tag(tree, block, arg_node);
        std::cout << "4\n";
    }
    std::cerr << "SSSSS";
    // rapidxml::xml_node<>* vertices_node = block_node->first_node("vertices");
    for (rapidxml::xml_node<>* vertex_node = block_node->first_node("vertex"); vertex_node;
         vertex_node = vertex_node->next_sibling()) {
        std::cerr << "SSSS1";
        block.handle_vertex_tag(tree, block, vertex_node);
        std::cerr << "SSSS2";
    }
    return block;
}

void DOM_tree::handle_tree(DOM_tree& tree) {
    handle_params_tag();
    rapidxml::xml_node<>* blocks_node = get_root_node()->first_node("blocks");
    for (rapidxml::xml_node<>* block_node = blocks_node->first_node("block"); block_node;
         block_node = block_node->next_sibling()) {
        std::string block_id = block_node->first_attribute("id")->value();
        int block_dim = std::stoi(block_node->first_attribute("dims")->value());
        printf("\nblock %s (%sD)\n",
               block_node->first_attribute("id")->value(),  // может не быть
               block_node->first_attribute("dims")->value());
        Block block = handle_block_tag(tree, block_node);
        blocks_.push_back(block);
    }
}

void DOM_tree::json_traverse() {
    json_traverse_rec(doc_json_);
}

void DOM_tree::json_traverse_rec(const Value& v) {
    // StringBuffer sb;
    // p.Stringify(sb);
    // std::cout << sb.GetString() << std::endl;

    switch (v.GetType()) {
        case kArrayType:
            for (SizeType i = 0; i != v.Size(); ++i)
                json_traverse(v[i]);
            break;
        case kObjectType:
            for (Value::ConstMemberIterator m = v.MemberBegin(); m != v.MemberEnd(); ++m)
                json_traverse(m->value);
            break;
        default:
            break;
    }
}

int main() {
    DOM_tree tree;
    tree.parse_xml2json("test.xml");
    tree.print_tree();
    tree.print_json();
    tree.json_traverse();
    // tree.handle_tree(tree);
    // tree.print_params_map();
    // tree.print_blocks();

    // const auto& blocks = tree.get_blocks();
    // Graph graph;
    // graph.vertices_coords_calc(blocks);
    // graph.print_vertices();
    // std::cout << graph.to_json_format();
}

// учитывать отсутствие айди у блока
// ввести обработку тпов вершин
// изменить формат хранения диапазонов переменных
// исключить дубликацию кода в вычислении координат вершин
// убрать запятую в последней строке вывода json
// сделать запись в файл
// подумать, надо ли менять логику айдишников
// избавиться от зависимости от ijk
// трёхмерные проекции многомерных вариантов
