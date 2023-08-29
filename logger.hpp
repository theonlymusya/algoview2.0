#pragma once
#include <fstream>
#include <vector>
#include "string"

namespace logger {

class Logger {
   public:
    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    void set_outdata_flow(std::string& file_name);

    void log_file_enter(const std::string& func_name, const std::string& file_name);

    void log_file_exit(const std::string& func_name, const std::string& file_name);

    void log_info_msg(const std::string& msg);

    void log_info_start_msg(const std::string& msg);

    void log_info_finish_msg(const std::string& msg);

    void log_char_msg(const std::string& msg, const char* detail);

    void log_err_msg(const std::string& func_name, const std::string& file_name, const std::string& msg);

    void log_warn_msg(const std::string& func_name, const std::string& file_name, const std::string& msg);

    void add_user_error(std::string error) { user_errors_.push_back(error); };

    void add_user_warning(std::string warning) { user_warnings_.push_back(warning); };

    std::string err_to_json();

    std::string warn_to_json();

   private:
    Logger();
    ~Logger();
    std::ofstream log_file_;
    const std::string info_log = "[INFO]: ";
    const std::string err_log = "[ERROR]: ";
    const std::string warn_log = "[WARNING]: ";
    const std::string func_in_log = "Enter function ";
    const std::string func_out_log = "Exit function ";
    const std::string file_log = " In file ";
    const std::string func_log = "In func ";
    std::vector<std::string> user_errors_;
    std::vector<std::string> user_warnings_;
};

}  // namespace logger
