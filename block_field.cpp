#include "block_field.hpp"
#include <iostream>
#include "expr.hpp"
#include "json_traverser.hpp"

namespace graph {
using namespace graph_info;
using namespace graph_manager;
using namespace algoview_json_traverser;
using namespace reg_expr;
using VarsMap = std::map<std::string, double>;

Block::Block(const BlockTagInfo& _block_info, int _block_number) {
    const auto& args = _block_info.get_args();
    const auto& arg = args.get_args();
    i_range = arg[0].end - arg[0].begin;
    j_range = arg[1].end - arg[1].begin;
    k_range = arg[2].end - arg[2].begin;
    std::cerr << std::endl;
    std::cerr << "i range " << i_range;
    std::cerr << "j range " << j_range;
    std::cerr << "k range " << k_range;
    std::cerr << std::endl;
    coords_field_.resize(i_range + 1);
    for (int i = 0; i <= i_range; i++) {
        coords_field_[i].resize(j_range + 1);
        for (int j = 0; j <= j_range; j++) {
            coords_field_[i][j].resize(k_range + 1, -1);
        }
    }
    for (int i = 0; i <= i_range; i++)
        for (int j = 0; j <= j_range; j++)
            for (int k = 0; k <= k_range; k++)
                std::cout << coords_field_[i][j][k] << " ";
    i_shift_ = 0 - arg[0].begin;
    j_shift_ = 0 - arg[1].begin;
    k_shift_ = 0 - arg[2].begin;
    dim = _block_info.dim;
    block_number = _block_number;
}

VertexId Block::get_vertex_id(CoordType i, CoordType j, CoordType k) {
    std::cerr << "             i " << i << " i_shift " << i_shift_ << std::endl;
    std::cerr << "             j " << j << " j_shift " << j_shift_ << std::endl;
    std::cerr << "             k " << k << " k_shift " << k_shift_ << std::endl;
    if (i + i_shift_ < 0 || j + j_shift_ < 0 || k + k_shift_ < 0 || i + i_shift_ > coords_field_.size() ||
        j + j_shift_ > coords_field_[0].size() || k + k_shift_ > coords_field_[0][0].size())
        return ignore_vertex_id;
    std::cerr << "vertex id " << coords_field_[i + i_shift_][j + j_shift_][k + k_shift_] << std::endl;
    return coords_field_[i + i_shift_][j + j_shift_][k + k_shift_];
}

VertexId Block::create_vertex(VertexMapManager& vertices_manager,
                              BlockId block_id,
                              CoordType i,
                              CoordType j,
                              CoordType k,
                              std::string type = "0") {
    Vertex* new_vertex_ptr =
        new Vertex{block_id, i + i_shift_, j + j_shift_, k + k_shift_ + block_number * block_number_shift_, type};
    VertexId new_vertex_id = vertices_manager.add_vertex(new_vertex_ptr);
    std::cerr << "new vertex id " << new_vertex_id << std::endl;
    std::cerr << "i " << i << " i_shift " << i_shift_ << std::endl;
    std::cerr << "j " << j << " j_shift " << j_shift_ << std::endl;
    std::cerr << "k " << k << " k_shift " << k_shift_ << std::endl;
    std::cerr << coords_field_.size();
    coords_field_[i + i_shift_][j + j_shift_][k + k_shift_] = new_vertex_id;
    for (int i = 0; i <= i_range; i++)
        for (int j = 0; j <= j_range; j++)
            for (int k = 0; k <= k_range; k++)
                std::cout << coords_field_[i][j][k] << " ";
    std::cerr << std::endl;
    std::cerr << "new vertex id " << coords_field_[i + i_shift_][j + j_shift_][k + k_shift_] << std::endl;
    std::cerr << "21";
    return new_vertex_id;
}

VertexId Block::get_or_create_source_vertex(VertexMapManager& vertices_manager,
                                            BlockId block_id,
                                            CoordType i,
                                            CoordType j,
                                            CoordType k) {
    std::cerr << "meow" << std::endl;
    std::cerr << "i " << i << std::endl;
    std::cerr << "j " << j << std::endl;
    std::cerr << "k " << k << std::endl;
    VertexId vertex_id = get_vertex_id(i, j, k);
    if (vertex_id == ignore_vertex_id)
        return ignore_vertex_id;
    if (vertex_id != -1)
        return vertex_id;
    vertex_id = create_vertex(vertices_manager, block_id, i, j, k, "0");
    return vertex_id;
}

void Block::create_edge(VertexId src_id, VertexId target_id, EdgeMapManager& edges_manager) {
    std::cerr << "Я создаю связь между " << src_id << " и " << target_id << std::endl;
    Edge* new_edge_ptr = new Edge{src_id, target_id};
    edges_manager.add_edge(new_edge_ptr);
}

void create_var_value_map(const ParamsMap& params_map, VarsMap& vars_map) {
    for (const auto& param : params_map) {
        vars_map[param.first] = param.second;
    }
}

void change_var_value_map(std::string var_name, int var_value, VarsMap& vars_map) {
    vars_map[var_name] = var_value;
}

void get_src_vertex_coords(const std::string& src_string,
                           int dim,
                           CoordType& i,
                           CoordType& j,
                           CoordType& k,
                           const VarsMap& vars_map) {
    std::string i_str, j_str, k_str, jk_str;
    if (dim == 1) {
        i = calc_expr(src_string, vars_map);
        j = 0;
        k = 0;
    } else if (dim == 2) {
        parse_value(src_string, i_str, j_str, ',');
        i = calc_expr(i_str, vars_map);
        j = calc_expr(j_str, vars_map);
        k = 0;
    } else if (dim == 3) {
        parse_value(src_string, i_str, jk_str, ',');
        parse_value(jk_str, j_str, k_str, ',');
        i = calc_expr(i_str, vars_map);
        j = calc_expr(j_str, vars_map);
        k = calc_expr(k_str, vars_map);
    }
    std::cerr << "i src" << i << std::endl;
    std::cerr << "j src" << j << std::endl;
    std::cerr << "k src" << k << std::endl;
}

int get_max(const std::vector<int>& levels) {
    int max_level = 0;
    for (auto level : levels) {
        if (level > max_level)
            max_level = level;
    }
    return max_level;
}

void Block::main_cycle(const BlockTagInfo& block_info,
                       const ParamsMap& params,
                       VertexMapManager& vertices_manager,
                       EdgeMapManager& edges_manager,
                       std::map<BlockId, Block*>& block_map) {
    const auto& args = block_info.get_args();
    const auto& arg = args.get_args();
    const auto& vertices = block_info.get_vertices();
    BlockId block_id = block_info.id;
    VarsMap vars_map;
    std::cerr << "6";
    create_var_value_map(params, vars_map);
    std::cerr << "7";
    for (int i = arg[0].begin; i <= arg[0].end; i++) {
        for (int j = arg[1].begin; j <= arg[1].end; j++) {
            for (int k = arg[2].begin; k <= arg[2].end; k++) {
                std::cerr << "8";
                for (const auto& vertex : vertices.get_vertices()) {
                    std::cerr << "9";
                    //{
                    if (arg[0].name != "_")
                        change_var_value_map(arg[0].name, i, vars_map);
                    if (arg[1].name != "_")
                        change_var_value_map(arg[1].name, j, vars_map);
                    if (arg[2].name != "_")
                        change_var_value_map(arg[2].name, k, vars_map);
                    //}
                    std::cerr << "10" << std::endl;
                    std::cerr << calc_expr(vertex.cond, vars_map) << std::endl;
                    if (calc_expr(vertex.cond, vars_map)) {
                        std::cerr << "11";
                        VertexId vertex_id = create_vertex(vertices_manager, block_id, i, j, k, vertex.type);
                        std::cerr << "12";
                        std::vector<int> src_vertices_levels;
                        for (const auto& src : vertex.src) {
                            std::cerr << "13";
                            int src_i, src_j, src_k;
                            get_src_vertex_coords(src, block_info.dim, src_i, src_j, src_k, vars_map);
                            std::cerr << "i src" << src_i << std::endl;
                            std::cerr << "j src" << src_j << std::endl;
                            std::cerr << "k src" << src_k << std::endl;
                            VertexId src_vertex_id =
                                get_or_create_source_vertex(vertices_manager, block_id, src_i, src_j, src_k);
                            // src_vertices_levels.push_back(vertices_manager.get_vertex_level(src_vertex_id));
                            if (src_vertex_id != ignore_vertex_id) {
                                std::cerr << "Я должен создать связь" << std::endl;
                                src_vertices_levels.push_back(vertices_manager.get_vertex_level(src_vertex_id));
                                create_edge(src_vertex_id, vertex_id, edges_manager);
                            }
                        }
                        for (const auto& bsrc : vertex.bsrc) {
                            BlockId bsrc_block_id = bsrc.first;
                            if (block_map.empty()) {
                                assert("Неверный порядок объявляемых блоков");
                            }
                            auto bsrc_block_ptr = block_map[bsrc_block_id];
                            int bsrc_dim = bsrc_block_ptr->dim;
                            int bsrc_i, bsrc_j, bsrc_k;
                            get_src_vertex_coords(bsrc.second, bsrc_dim, bsrc_i, bsrc_j, bsrc_k, vars_map);
                            std::cerr << "i bsrc" << bsrc_i << std::endl;
                            std::cerr << "j bsrc" << bsrc_j << std::endl;
                            std::cerr << "k bsrc" << bsrc_k << std::endl;
                            VertexId bsrc_vertex_id = bsrc_block_ptr->get_or_create_source_vertex(
                                vertices_manager, bsrc_block_id, bsrc_i, bsrc_j, bsrc_k);
                            // src_vertices_levels.push_back(vertices_manager.get_vertex_level(bsrc_vertex_id));
                            if (bsrc_vertex_id != ignore_vertex_id) {
                                std::cerr << "Я должен создать связь" << std::endl;
                                src_vertices_levels.push_back(vertices_manager.get_vertex_level(bsrc_vertex_id));
                                create_edge(bsrc_vertex_id, vertex_id, edges_manager);
                            }
                        }
                        if (!src_vertices_levels.empty()) {
                            int level = get_max(src_vertices_levels) + 1;
                            vertices_manager.add_vertex_level(vertex_id, level);
                        }
                    }
                }
            }
        }
    }
}

}  // namespace graph
