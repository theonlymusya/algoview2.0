#pragma once
#include <string>
#include <vector>

namespace graph_info {
struct ArgTagInfo {
    std::string name;
    int begin;
    int end;
};

class ArgTagsInfo {
   public:
    ArgTagsInfo();

    void add_arg(std::string name, int begin_value, int end_value);
    int is_arg_name_unique(std::string name);
    void print_arg_tags() const;
    const std::vector<ArgTagInfo>& get_args() const;

   private:
    int n_ = 0;
    std::vector<ArgTagInfo> arg_tags_;
};
}  // namespace graph_info
