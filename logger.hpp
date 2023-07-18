#pragma once
#include "string"

namespace logger {
const std::string info_log = "[INFO]: ";
const std::string err_log = "[ERROR]: ";
const std::string warn_log = "[WARNING]: ";
const std::string func_in_log = "Enter function ";
const std::string func_out_log = "Exit function ";
const std::string file_log = " In file ";
const std::string func_log = "In func ";

inline void log_file_enter(const std::string& func_name, const std::string& file_name) {
    std::cerr << info_log << func_in_log << func_name << file_log << file_name << std::endl;
}

inline void log_file_exit(const std::string& func_name, const std::string& file_name) {
    std::cerr << info_log << func_out_log << func_name << file_log << file_name << std::endl;
}

inline void log_info_msg(const std::string& msg) {
    std::cerr << info_log << msg << std::endl;
}

inline void log_info_start_msg(const std::string& msg) {
    std::cerr << info_log << "Start " << msg << std::endl;
}

inline void log_info_finish_msg(const std::string& msg) {
    std::cerr << info_log << "Finish " << msg << " sucessfully!" << std::endl;
}

inline void log_char_msg(const std::string& msg, const char* detail) {
    std::cerr << info_log << msg;
    fprintf(stderr, "%s\n", detail);
}

inline void log_err_msg(const std::string& func_name, const std::string file_name, const std::string& msg) {
    std::cerr << err_log << func_log << func_name << file_log << file_name << ": " << msg << std::endl;
}
}  // namespace logger
