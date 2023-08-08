#include "block_field.hpp"
#include <iostream>
#include "expr.hpp"
#include "json_traverser.hpp"
#include "logger.hpp"

const std::string file_name = "block_field.cpp";

namespace graph {
using namespace graph_info;
using namespace graph_manager;
using namespace algoview_json_traverser;
using namespace reg_expr;
using namespace logger;
using VarsMap = std::map<std::string, double>;
int cur_block_shift = 0;

Block::Block(const BlockTagInfo& _block_info, int _block_number) {
    const auto& args = _block_info.get_args();
    const auto& arg = args.get_args();
    i_range = arg[0].end - arg[0].begin;
    j_range = arg[1].end - arg[1].begin;
    k_range = arg[2].end - arg[2].begin;
    coords_field_.resize(i_range + 1);
    for (int i = 0; i <= i_range; i++) {
        coords_field_[i].resize(j_range + 1);
        for (int j = 0; j <= j_range; j++) {
            coords_field_[i][j].resize(k_range + 1, -1);
        }
    }
    // for (int i = 0; i <= i_range; i++)
    //     for (int j = 0; j <= j_range; j++)
    //         for (int k = 0; k <= k_range; k++)
    //             std::cout << coords_field_[i][j][k] << " ";
    i_shift_ = 0 - arg[0].begin;
    j_shift_ = 0 - arg[1].begin;
    k_shift_ = 0 - arg[2].begin;
    dim = _block_info.dim;
    block_number = _block_number;
    if (dim == 3)
        local_block_shift_ = k_range + 1;
    cur_block_shift += local_block_shift_;
}

VertexId Block::get_vertex_id(CoordType i, CoordType j, CoordType k) {
    const std::string func_name = "get_vertex_id";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("finding vertex id");
    std::string msg = "i = " + std::to_string(i) + " i_shift = " + std::to_string(i_shift_);
    logger.log_info_msg(msg);
    msg = "j = " + std::to_string(j) + " j_shift = " + std::to_string(j_shift_);
    logger.log_info_msg(msg);
    msg = "k = " + std::to_string(j) + " k_shift = " + std::to_string(k_shift_);
    logger.log_info_msg(msg);

    if (i + i_shift_ < 0 || j + j_shift_ < 0 || k + k_shift_ < 0 || i + i_shift_ >= coords_field_.size() ||
        j + j_shift_ >= coords_field_[0].size() || k + k_shift_ >= coords_field_[0][0].size()) {
        logger.log_warn_msg(func_name, file_name, "Vertex with current coords will be ignored");
        return ignore_vertex_id;
    }

    msg = "Founded vertex id = " + coords_field_[i + i_shift_][j + j_shift_][k + k_shift_];
    logger.log_file_exit(func_name, file_name);
    return coords_field_[i + i_shift_][j + j_shift_][k + k_shift_];
}

VertexId Block::create_vertex(VertexMapManager& vertices_manager,
                              BlockId block_id,
                              CoordType i,
                              CoordType j,
                              CoordType k,
                              std::string type = "0") {
    const std::string func_name = "create_vertex";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("creating new vertex");
    logger.log_info_msg("i = " + std::to_string(i) + " i_shift = " + std::to_string(i_shift_));
    logger.log_info_msg("j = " + std::to_string(j) + " j_shift = " + std::to_string(j_shift_));
    logger.log_info_msg("k = " + std::to_string(j) + " k_shift = " + std::to_string(k_shift_));

    Vertex* new_vertex_ptr =
        new Vertex{block_id, i + i_shift_, j + j_shift_, k + k_shift_ + (cur_block_shift - local_block_shift_), type};
    VertexId new_vertex_id = vertices_manager.add_vertex(new_vertex_ptr);
    logger.log_info_msg("New vertex = " + std::to_string(new_vertex_id));

    // std::cerr << coords_field_.size();
    coords_field_[i + i_shift_][j + j_shift_][k + k_shift_] = new_vertex_id;
    // for (int i = 0; i <= i_range; i++)
    //     for (int j = 0; j <= j_range; j++)
    //         for (int k = 0; k <= k_range; k++)
    //             std::cout << coords_field_[i][j][k] << " ";
    logger.log_info_finish_msg("creating new vertex");
    logger.log_file_enter(func_name, file_name);
    return new_vertex_id;
}

VertexId Block::get_or_create_source_vertex(VertexMapManager& vertices_manager,
                                            BlockId block_id,
                                            CoordType i,
                                            CoordType j,
                                            CoordType k) {
    const std::string func_name = "get_or_create_source_vertex";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    std::string msg = "Get or create source vertex with coords : " + std::to_string(i) + " " + std::to_string(j) + " " +
                      std::to_string(k);
    logger.log_info_msg(msg);
    VertexId vertex_id = get_vertex_id(i, j, k);
    if (vertex_id == ignore_vertex_id) {
        logger.log_warn_msg(func_name, file_name, "Src vertex is ignored");
        return ignore_vertex_id;
    }
    if (vertex_id != -1) {
        logger.log_warn_msg(func_name, file_name, "Src vertex will be created as i/o vertex");
        return vertex_id;
    }
    vertex_id = create_vertex(vertices_manager, block_id, i, j, k, "0");

    logger.log_file_enter(func_name, file_name);
    return vertex_id;
}

void Block::create_edge(VertexId src_id, VertexId target_id, EdgeMapManager& edges_manager) {
    const std::string func_name = "create_edge";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("creating edge");
    logger.log_info_msg("src vertex = " + std::to_string(src_id) + "target vertex id = " + std::to_string(target_id));

    Edge* new_edge_ptr = new Edge{src_id, target_id};
    edges_manager.add_edge(new_edge_ptr);

    logger.log_info_finish_msg("creating edge");
    logger.log_file_exit(func_name, file_name);
}

void create_var_value_map(const ParamsMap& params_map, VarsMap& vars_map) {
    const std::string func_name = "create_var_value_map";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("creating var_value map");

    for (const auto& param : params_map) {
        vars_map[param.first] = param.second;
    }

    logger.log_file_exit(func_name, file_name);
    logger.log_info_finish_msg("creating var_value map");
}

void change_var_value_map(std::string var_name, int var_value, VarsMap& vars_map) {
    const std::string func_name = "change_var_value_map";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("adding new variable to var_value map");
    std::string msg = var_name + " = " + std::to_string(var_value);
    logger.log_info_msg(msg);

    vars_map[var_name] = var_value;

    logger.log_info_finish_msg("adding new variable to var_value map");
    logger.log_file_exit(func_name, file_name);
}

void get_src_vertex_coords(const std::string& src_string,
                           int dim,
                           CoordType& i,
                           CoordType& j,
                           CoordType& k,
                           const VarsMap& vars_map) {
    const std::string func_name = "get_src_vertex_coords";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("getting scr vertex coords values");
    std::string msg = "source block dimentional = " + dim;
    logger.log_info_msg(msg);

    std::string i_str, j_str, k_str, jk_str;
    if (dim == 1) {
        i = calc_expr(src_string, vars_map);
        j = 0;
        k = 0;
    } else if (dim == 2) {
        logger.log_info_msg("Parsing by , step 1");
        parse_value(src_string, i_str, j_str, ',');
        msg = "i_string = " + i_str + "#";
        logger.log_info_msg(msg);
        msg = "j_string = " + j_str + "#";
        logger.log_info_msg(msg);
        i = calc_expr(i_str, vars_map);
        j = calc_expr(j_str, vars_map);
        k = 0;
    } else if (dim == 3) {
        logger.log_info_msg("Parsing by , step 1");
        parse_value(src_string, i_str, jk_str, ',');
        msg = "i_string = " + i_str + "#";
        logger.log_info_msg(msg);
        msg = "jk_string = " + jk_str + "#";
        logger.log_info_msg(msg);

        logger.log_info_msg("Parsing by , step 2");
        parse_value(jk_str, j_str, k_str, ',');
        msg = "i_string = " + i_str + "#";
        logger.log_info_msg(msg);
        msg = "j_string = " + j_str + "#";
        logger.log_info_msg(msg);
        msg = "k_string = " + k_str + "#";
        logger.log_info_msg(msg);

        i = calc_expr(i_str, vars_map);
        j = calc_expr(j_str, vars_map);
        k = calc_expr(k_str, vars_map);
    }

    msg = "i src coord = " + std::to_string(i) + "\tj src coord = " + std::to_string(j) +
          "\tk src coord = " + std::to_string(k);
    logger.log_info_msg(msg);
    logger.log_info_finish_msg("getting scr vertex coords values");
    logger.log_file_exit(func_name, file_name);
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
    const std::string func_name = "create_var_value_map";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("converting block to part of graph");
    logger.log_info_msg("BLOCK " + std::to_string(block_info.id));

    const auto& args = block_info.get_args();
    const auto& arg = args.get_args();
    const auto& vertices = block_info.get_vertices();
    BlockId block_id = block_info.id;
    VarsMap vars_map;
    create_var_value_map(params, vars_map);
    logger.log_info_start_msg("iterating by args");
    for (int i = arg[0].begin; i <= arg[0].end; i++) {
        for (int j = arg[1].begin; j <= arg[1].end; j++) {
            for (int k = arg[2].begin; k <= arg[2].end; k++) {
                logger.log_info_msg("Got current arg values");
                std::string msg = arg[0].name + " = " + std::to_string(i) + "\t" + arg[1].name + " = " +
                                  std::to_string(j) + "\t" + arg[2].name + " = " + std::to_string(k) + "\t";
                logger.log_info_msg(msg);
                logger.log_info_start_msg("iterating verticies");
                for (const auto& vertex : vertices.get_vertices()) {
                    //{
                    if (arg[0].name != "_")
                        change_var_value_map(arg[0].name, i, vars_map);
                    if (arg[1].name != "_")
                        change_var_value_map(arg[1].name, j, vars_map);
                    if (arg[2].name != "_")
                        change_var_value_map(arg[2].name, k, vars_map);
                    //}
                    logger.log_info_start_msg("calculating condition");
                    if (calc_expr(vertex.cond, vars_map)) {
                        VertexId vertex_id = create_vertex(vertices_manager, block_id, i, j, k, vertex.type);
                        std::vector<int> src_vertices_levels;
                        logger.log_info_start_msg("iterating src");
                        for (const auto& src : vertex.src) {
                            int src_i, src_j, src_k;
                            get_src_vertex_coords(src, block_info.dim, src_i, src_j, src_k, vars_map);
                            VertexId src_vertex_id =
                                get_or_create_source_vertex(vertices_manager, block_id, src_i, src_j, src_k);
                            // src_vertices_levels.push_back(vertices_manager.get_vertex_level(src_vertex_id));
                            if (src_vertex_id != ignore_vertex_id) {
                                src_vertices_levels.push_back(vertices_manager.get_vertex_level(src_vertex_id));
                                create_edge(src_vertex_id, vertex_id, edges_manager);
                            }
                        }
                        logger.log_info_finish_msg("iterating src");
                        logger.log_info_start_msg("iterating bsrc");
                        for (const auto& bsrc : vertex.bsrc) {
                            BlockId bsrc_block_id = bsrc.first;
                            if (block_map.empty()) {
                                // assert("Неверный порядок объявляемых блоков");
                                logger.log_err_msg(func_name, file_name, "Source block is not defined yet");
                                exit(1);
                            }
                            auto bsrc_block_ptr = block_map[bsrc_block_id];
                            int bsrc_dim = bsrc_block_ptr->dim;
                            int bsrc_i, bsrc_j, bsrc_k;
                            get_src_vertex_coords(bsrc.second, bsrc_dim, bsrc_i, bsrc_j, bsrc_k, vars_map);
                            VertexId bsrc_vertex_id = bsrc_block_ptr->get_or_create_source_vertex(
                                vertices_manager, bsrc_block_id, bsrc_i, bsrc_j, bsrc_k);
                            // src_vertices_levels.push_back(vertices_manager.get_vertex_level(bsrc_vertex_id));
                            if (bsrc_vertex_id != ignore_vertex_id) {
                                src_vertices_levels.push_back(vertices_manager.get_vertex_level(bsrc_vertex_id));
                                create_edge(bsrc_vertex_id, vertex_id, edges_manager);
                            }
                        }
                        logger.log_info_finish_msg("iterating bsrc");
                        if (!src_vertices_levels.empty()) {
                            logger.log_info_start_msg("determining vertex level");
                            int level = get_max(src_vertices_levels) + 1;
                            vertices_manager.add_vertex_level(vertex_id, level);
                            logger.log_info_finish_msg("determining vertex level");
                        }
                    }
                }
                logger.log_info_finish_msg("iterating verticies");
            }
        }
    }
    logger.log_info_finish_msg("iterating by args");
    logger.log_info_finish_msg("converting block to part of graph");
    logger.log_file_exit(func_name, file_name);
}

}  // namespace graph
