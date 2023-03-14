#include "graph_info.hpp"
#include <iostream>

namespace graph_info {
void GraphInfo::add_param(std::string name, int value) {
    params_[name] = value;
}

void GraphInfo::print_params() const {
    std::cout << "\t\t\tPARAMS" << std::endl;
    for (const auto& param : params_) {
        std::cout << param.first << " = " << param.second << std::endl;
    }
}

void GraphInfo::print_graph() const {
    print_params();
    blocks_.print_block_tags();
}

ParamsMap& GraphInfo::get_params() {
    return params_;
}

BlockTagsInfo& GraphInfo::get_blocks() {
    return blocks_;
}
}  // namespace graph_info

int main() {
    using namespace graph_info;
    GraphInfo graph;
    BlockTagsInfo& blocks = graph.get_blocks();
    graph.add_param("N", 10);
    graph.add_param("M", 9);
    blocks.new_block();
    blocks.add_id("1");
    blocks.add_dim(3);
    auto& block = blocks.get_last_block();
    auto& args = block.get_args();
    auto& vertices = block.get_vertices();
    vertices.new_vertex();
    vertices.add_type("1");
    vertices.add_condition("meow");
    vertices.add_src("1 + 2");
    vertices.add_bsrc(std::make_pair("2 + 2", "j"));
    args.add_arg("i", 0, 10);
    args.add_arg("j", 0, 10);
    blocks.new_block();
    blocks.add_id("0");
    blocks.add_dim(3);
    auto& block1 = blocks.get_last_block();
    auto& args1 = block1.get_args();
    auto& vertices1 = block1.get_vertices();
    vertices1.new_vertex();
    vertices1.add_type("1");
    vertices1.add_condition("meow");
    vertices1.add_src("1 + 2");
    vertices1.add_bsrc(std::make_pair("2 + 2", "j"));
    args1.add_arg("i", 0, 10);
    args1.add_arg("j", 0, 10);
    graph.print_graph();
    // blocks.print_block_tags();
}
