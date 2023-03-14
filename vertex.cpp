#include "vertex.hpp"
#include <iostream>

namespace graph_info {

void VertexTagsInfo::new_vertex() {
    vertex_tags_.emplace_back(VertexTagInfo{});
    n_++;
}

void VertexTagsInfo::add_condition(std::string cond) {
    vertex_tags_[n_].cond = cond;
}

void VertexTagsInfo::add_type(std::string type) {
    vertex_tags_[n_].type = type;
}

void VertexTagsInfo::add_src(std::string src) {
    vertex_tags_[n_].src.push_back(src);
}

void VertexTagsInfo::add_bsrc(std::pair<std::string, std::string> bsrc) {
    vertex_tags_[n_].bsrc.push_back(bsrc);
}

void VertexTagsInfo::print_vertex_tags() const {
    for (const auto& vertex : vertex_tags_) {
        std::cout << "type = " << vertex.type << std::endl;
        std::cout << "condition = " << vertex.cond << std::endl;
        if (!vertex.src.empty()) {
            for (const auto& src_elem : vertex.src) {
                std::cout << "src:\t" << src_elem << std::endl;
            }
        }
        if (!vertex.bsrc.empty()) {
            for (const auto& bsrc_elem : vertex.bsrc) {
                std::cout << "bsrc:\t" << bsrc_elem.first << "\tsrc:\t" << bsrc_elem.second << std::endl;
            }
        }
        std::cout << std::endl;
    }
}
}  // namespace graph_info
