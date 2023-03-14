#pragma once
#include <string>
#include <vector>

namespace graph_info {

using VertexId = int;
using VertexType = std::string;

struct VertexTagInfo {
    VertexType type = "1";
    std::string cond = "";
    std::vector<std::string> src;
    std::vector<std::string> bsrc;
    // std::vector<std::pair<std::string, std::string>> bsrc;
};

class VertexTagsInfo {
   public:
    void add_vertex(VertexType type, std::string cond, std::vector<std::string>& src);
    void add_vertex(VertexType type, std::string cond, std::vector<std::string>& src, std::vector<std::string>& bsrc);
    void new_vertex();
    // void add_condtion();
    // void add_type();
    // void add_bsrc();
    // void add_src();
    void print_vertex_tags() const;

   private:
    int n_ = 0;
    std::vector<VertexTagInfo> vertex_tags_;
};
}  // namespace graph_info