#pragma once
#include "argument.hpp"
#include "vertex.hpp"

namespace graph_info {
using BlockId = std::string;
using Dim = int;

// struct BlockTagInfo {
//     BlockId id = "0";
//     Dim dim = 0;
//     ArgTagsInfo args_ = ArgTagsInfo();
//     VertexTagsInfo vertices_;
// }

class BlockTagInfo {
   public:
    BlockId id = "0";
    Dim dim = 0;
    void print_block_tag() const;
    VertexTagsInfo& get_vertices();
    ArgTagsInfo& get_args();

   private:
    ArgTagsInfo args_;
    VertexTagsInfo vertices_;
};
}  // namespace graph_info