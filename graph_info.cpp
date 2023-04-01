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
