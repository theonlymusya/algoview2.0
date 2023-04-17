#pragma once
#include <string>
#include <vector>

namespace graph_info {
using VertexType = std::string;

struct VertexTagInfo {
    VertexType type = "1";
    std::string cond = "";
    std::vector<std::string> src;
    std::vector<std::pair<int, std::string>> bsrc;
};

class VertexTagsInfo {
   public:
    void new_vertex();
    void add_condition(std::string cond);
    void add_type(std::string type);
    void add_src(std::string src);
    void add_bsrc(std::pair<int, std::string> bsrc);
    const std::vector<VertexTagInfo>& get_vertices() const;

    void print_vertex_tags() const;

   private:
    int n_ = -1;
    std::vector<VertexTagInfo> vertex_tags_;
};
}  // namespace graph_info