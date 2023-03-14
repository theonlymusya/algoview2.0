#include "argument.hpp"
#include <cassert>
#include <iostream>

namespace graph_info {
ArgTagsInfo::ArgTagsInfo() {
    for (int i = 0; i < 3; i++) {
        arg_tags_.push_back(ArgTagInfo{"_", 0, 0});
    }
}

int ArgTagsInfo::is_arg_name_unique(std::string name) {
    for (const auto& arg : arg_tags_) {
        if (arg.name == name)
            return 0;
    }
    return 1;
}

void ArgTagsInfo::add_arg(std::string name, int begin_value, int end_value) {
    assert((n_ < 3) && "3D graph only");
    assert(is_arg_name_unique(name) && "Имя аргумента используется повторно");
    arg_tags_[n_] = ArgTagInfo{name, begin_value, end_value};
    n_++;
}

void ArgTagsInfo::print_arg_tags() const {
    for (const auto& arg : arg_tags_) {
        std::cout << "name = " << arg.name << std::endl;
        std::cout << "begin = " << arg.begin << std::endl;
        std::cout << "end = " << arg.end << std::endl;
        std::cout << std::endl;
    }
}
}  // namespace graph_info