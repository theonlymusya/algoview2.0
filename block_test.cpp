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

int main() {
    using namespace graph_info;
    BlockTagInfo block;
    auto& args = block.get_args();
    auto& vertices = block.get_vertices();
    vertices.new_vertex();
    vertices.add_type("1");
    vertices.add_condition("meow");
    vertices.add_src("1 + 2");
    vertices.add_bsrc(std::make_pair("2 + 2", "j"));
    args.add_arg("i", 0, 10);
    args.add_arg("j", 0, 10);
    block.print_block_tag();
}
