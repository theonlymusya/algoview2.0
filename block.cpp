#include "block.hpp"
#include "iostream"

namespace graph_info {
ArgTagsInfo& BlockTagInfo::get_args() {
    return args_;
}

VertexTagsInfo& BlockTagInfo::get_vertices() {
    return vertices_;
}

void BlockTagInfo::print_block_tag() const {
    std::cout << "\t\t\tBLOCK TAG" << std::endl;
    std::cout << "id = " << id << std::endl;
    std::cout << "dim = " << dim << std::endl << std::endl;
    std::cout << "\t\t\tARG TAGS" << std::endl;
    args_.print_arg_tags();
    std::cout << "\t\t\tVERTEX TAGS" << std::endl;
    vertices_.print_vertex_tags();
}

}  // namespace graph_info
