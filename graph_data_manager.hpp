#pragma once
#include <map>

namespace graph_manager {
using VertexId = int;
using EdgeId = int;
using CoordType = int;

struct Vertex {
    int block_id;
    int i, j, k;
    std::string type = "0";
};

struct Edge {
    VertexId source_vertex_id, target_vertex_id;
    std::string type = "0";
};

using VertexMap = std::map<VertexId, const Vertex*>;
using EdgeMap = std::map<EdgeId, const Edge*>;

class VertexMapManager {
   public:
    VertexId add_vertex(const Vertex* vertex);
    std::string to_json();
    void clean_map();

   private:
    VertexId vertex_id_counter_ = 0;
    VertexId get_new_vertex_id() { return vertex_id_counter_++; }
    VertexMap vertices_;
};

class EdgeMapManager {
   public:
    void add_edge(const Edge* edge);
    std::string to_json();
    void clean_map();

   private:
    VertexId edge_id_counter_ = 0;
    VertexId get_new_edge_id() { return edge_id_counter_++; }
    EdgeMap edges_;
};

void print_json(VertexMapManager& vertices_manager, EdgeMapManager& edges_manager);
}  // namespace graph_manager
