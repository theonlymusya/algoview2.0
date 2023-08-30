#include "block.hpp"
#include "cassert"
#include "iostream"
#include "output_file_manager.hpp"

const std::string file_name = "block.cpp";

namespace graph_info {

using namespace logger;
using namespace output_file_manager;

ArgTagsInfo& BlockTagInfo::get_args() {
    return args_;
}

const ArgTagsInfo& BlockTagInfo::get_args() const {
    return args_;
}

VertexTagsInfo& BlockTagInfo::get_vertices() {
    return vertices_;
}

const VertexTagsInfo& BlockTagInfo::get_vertices() const {
    return vertices_;
}

void BlockTagInfo::print_block_tag() const {
    std::cout << std::endl << "\t\t\tBLOCK TAG" << std::endl;
    std::cout << "id = " << id << std::endl;
    std::cout << "dim = " << dim << std::endl << std::endl;
    std::cout << "\t\t\tARG TAGS" << std::endl;
    args_.print_arg_tags();
    std::cout << "\t\t\tVERTEX TAGS" << std::endl;
    vertices_.print_vertex_tags();
}

void BlockTagsInfo::new_block() {
    blocks_.emplace_back(BlockTagInfo());
    n_++;
}

int BlockTagsInfo::is_block_id_unique(BlockId id) {
    for (auto it = blocks_.begin(); it != blocks_.end() - 1; it++) {
        if ((*it).id == id)
            return 0;
    }
    return 1;
}

void BlockTagsInfo::add_id(BlockId id) {
    const std::string func_name = "add_id";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("adding block id to internal structure");

    if (!is_block_id_unique(id)) {
        logger.log_err_msg(func_name, file_name, "Block id is used twice");
        logger.add_user_error("Block id is used twice");
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }
    blocks_[n_].id = id;

    logger.log_info_finish_msg("adding block id to internal structure");
    logger.log_file_exit(func_name, file_name);
}

void BlockTagsInfo::add_dim(int dim) {
    const std::string func_name = "add_dim";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("adding block dim to internal structure");

    if (!(dim > 0 && dim < 4)) {
        std::string msg = "Invalid dimension of block " + std::to_string(blocks_[n_].id);
        logger.log_err_msg(func_name, file_name, msg);
        logger.add_user_error(msg);
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }
    blocks_[n_].dim = dim;

    logger.log_info_finish_msg("adding block dim to internal structure");
    logger.log_file_exit(func_name, file_name);
}

void BlockTagsInfo::print_block_tags() const {
    for (const auto& block : blocks_) {
        block.print_block_tag();
    }
}

BlockTagInfo& BlockTagsInfo::get_last_block() {
    return blocks_[n_];
}

const std::vector<BlockTagInfo>& BlockTagsInfo::get_blocks() {
    return blocks_;
}
}  // namespace graph_info
