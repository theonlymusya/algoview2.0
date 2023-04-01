#include "block.hpp"
#include "cassert"
#include "iostream"

namespace graph_info {
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
    assert(is_block_id_unique(id) && "ID блока используется повторно");
    blocks_[n_].id = id;
}

void BlockTagsInfo::add_dim(int dim) {
    assert((dim > 0 && dim < 4) && "Неверная размерность блока");
    blocks_[n_].dim = dim;
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
