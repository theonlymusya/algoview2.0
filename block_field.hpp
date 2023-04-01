#pragma once
#include "graph_data_manager.hpp"
#include "graph_info.hpp"

namespace graph {
using namespace graph_info;
using namespace graph_manager;

class Block {
   public:
    int dim = 0;
    int i_range = 0;
    int j_range = 0;
    int k_range = 0;
    Block(const BlockTagInfo& _block_info);
    VertexId get_vertex_id(CoordType i, CoordType j, CoordType k);
    VertexId get_or_create_source_vertex(VertexMapManager&, BlockId block_id, CoordType i, CoordType j, CoordType k);
    VertexId create_vertex(VertexMapManager&, BlockId, CoordType i, CoordType j, CoordType k, std::string type);
    void create_edge(VertexId src_id, VertexId target_id, EdgeMapManager&);
    void main_cycle(const BlockTagInfo&, const ParamsMap&, VertexMapManager&, EdgeMapManager&);

   private:
    int i_shift_ = 0;
    int j_shift_ = 0;
    int k_shift_ = 0;
    std::vector<std::vector<std::vector<CoordType>>> coords_field_;
};
}  // namespace graph
