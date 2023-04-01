#pragma once
#include <vector>
#include "argument.hpp"
#include "vertex.hpp"

namespace graph_info {
using BlockId = int;

class BlockTagInfo {
   public:
    BlockId id = 0;
    int dim = 0;
    void print_block_tag() const;
    VertexTagsInfo& get_vertices();
    ArgTagsInfo& get_args();
    const ArgTagsInfo& get_args() const;
    const VertexTagsInfo& get_vertices() const;

   private:
    ArgTagsInfo args_;
    VertexTagsInfo vertices_;
};

class BlockTagsInfo {
   public:
    void new_block();
    int is_block_id_unique(BlockId id);
    void add_id(BlockId id);
    void add_dim(int dim);
    void print_block_tags() const;
    BlockTagInfo& get_last_block();
    const std::vector<BlockTagInfo>& get_blocks();

   private:
    int n_ = -1;
    std::vector<BlockTagInfo> blocks_;
};
}  // namespace graph_info