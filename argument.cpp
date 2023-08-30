#include "argument.hpp"
#include <cassert>
#include <iostream>
#include "logger.hpp"
#include "output_file_manager.hpp"

const std::string file_name = "argument.cpp";

namespace graph_info {

using namespace logger;
using namespace output_file_manager;

const std::vector<ArgTagInfo>& ArgTagsInfo::get_args() const {
    return arg_tags_;
}

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
    const std::string func_name = "add_arg";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_start_msg("adding new argument to internal structure");
    if (n_ >= 3) {
        // std::cerr << n_;
        logger.log_err_msg(func_name, file_name, "Too many arguments");
        logger.add_user_error("Too many arguments: 3D graph only");
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }
    if (!is_arg_name_unique(name)) {
        logger.log_err_msg(func_name, file_name, "Arg name is used twice in the same block");
        logger.add_user_error("Arg name is used twice in the same block");
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }
    if (!(begin_value <= end_value)) {
        logger.log_err_msg(func_name, file_name, "Invalid arg value range");
        logger.add_user_error("Invalid argument value range: end value greater that begin value");
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }
    arg_tags_[n_] = ArgTagInfo{name, begin_value, end_value};
    n_++;

    logger.log_info_finish_msg("adding new argument to internal structure");
    logger.log_file_exit(func_name, file_name);
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
