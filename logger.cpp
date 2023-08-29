#include "logger.hpp"
#include <iostream>
#include <string>

namespace logger {

Logger::Logger() {}

void Logger::set_outdata_flow(std::string& file_name) {
    log_file_.open(file_name);
}

Logger::~Logger() {
    log_file_.close();
}

void Logger::log_file_enter(const std::string& func_name, const std::string& file_name) {
    log_file_ << info_log << func_in_log << func_name << file_log << file_name << std::endl;
}

void Logger::log_file_exit(const std::string& func_name, const std::string& file_name) {
    log_file_ << info_log << func_out_log << func_name << file_log << file_name << std::endl;
}

void Logger::log_info_msg(const std::string& msg) {
    log_file_ << info_log << msg << std::endl;
}

void Logger::log_info_start_msg(const std::string& msg) {
    log_file_ << info_log << "Start " << msg << std::endl;
}

void Logger::log_info_finish_msg(const std::string& msg) {
    log_file_ << info_log << "Finish " << msg << " sucessfully!" << std::endl;
}

void Logger::log_char_msg(const std::string& msg, const char* detail) {
    std::string str_detail(detail);
    log_file_ << info_log << msg << detail << std::endl;
    // fprintf(log_file_, "%s\n", detail);
}

void Logger::log_err_msg(const std::string& func_name, const std::string& file_name, const std::string& msg) {
    log_file_ << err_log << func_log << func_name << file_log << file_name << ": " << msg << std::endl;
}

void Logger::log_warn_msg(const std::string& func_name, const std::string& file_name, const std::string& msg) {
    log_file_ << warn_log << func_log << func_name << file_log << file_name << ": " << msg << std::endl;
}

std::string Logger::warn_to_json() {
    std::string result_string = "\t\"warnings\": [";
    for (const auto& warn : user_warnings_) {
        result_string += "\n\t\t\"";
        result_string += warn;
        result_string += "\",";
    }
    if (result_string.back() != '[') {
        result_string.pop_back();
        result_string += "\n\t],\n";
    } else {
        result_string += "],\n";
    }
    return result_string;
}

std::string Logger::err_to_json() {
    std::string result_string = "\t\"errors\": [";
    for (const auto& error : user_errors_) {
        result_string += "\n\t\t\"";
        result_string += error;
        result_string += "\",";
    }
    if (result_string.back() != '[') {
        result_string.pop_back();
        result_string += "\n\t]\n";
    } else {
        result_string += "]\n";
    }
    return result_string;
}
}  // namespace logger