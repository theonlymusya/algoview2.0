#include "logger.hpp"
#include <iostream>

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
}  // namespace logger