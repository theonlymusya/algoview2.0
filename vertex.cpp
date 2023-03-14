#include "vertex.hpp"
#include <iostream>

namespace graph_info {
void VertexTagsInfo::add_vertex(VertexType type, std::string cond, std::vector<std::string>& src) {
    vertex_tags_.emplace_back(VertexTagInfo{type, cond});
    for (auto src_elem : src) {
        vertex_tags_[n_].src.push_back(src_elem);
    }
    n_++;
}

void VertexTagsInfo::add_vertex(VertexType type,
                                std::string cond,
                                std::vector<std::string>& src,
                                std::vector<std::string>& bsrc) {
    vertex_tags_.emplace_back(VertexTagInfo{type, cond});
    for (auto src_elem : src) {
        vertex_tags_[n_].src.push_back(src_elem);
    }
    for (auto bsrc_elem : bsrc) {
        vertex_tags_[n_].bsrc.push_back(bsrc_elem);
    }
    n_++;
}

void VertexTagsInfo::print_vertex_tags() const {
    for (const auto& vertex : vertex_tags_) {
        std::cout << "type = " << vertex.type << std::endl;
        std::cout << "condition = " << vertex.cond << std::endl;
        std::cout << "src: " << std::endl;
        for (const auto& src_elem : vertex.src) {
            std::cout << "\t" << src_elem << std::endl;
        }
        if (!vertex.bsrc.empty()) {
            std::cout << "bsrc: " << std::endl;
            for (const auto& bsrc_elem : vertex.bsrc) {
                std::cout << "\t" << bsrc_elem << std::endl;
            }
        }
        std::cout << std::endl;
    }
}
}  // namespace graph_info
