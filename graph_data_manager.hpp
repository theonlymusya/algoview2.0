#pragma once
#include <map>
#include <string>
#include <vector>

namespace graph_manager {
using VertexId = int;
using EdgeId = int;
using CoordType = int;

struct Vertex {
    int block_id;
    int i, j, k;
    std::string type = "0";
    int level = 0;
};

struct Edge {
    VertexId source_vertex_id, target_vertex_id;
    std::string type = "0";
};

using VertexMap = std::map<VertexId, Vertex*>;
using EdgeMap = std::map<EdgeId, const Edge*>;

class VertexMapManager {
   public:
    VertexId add_vertex(Vertex* vertex);
    void add_vertex_level(VertexId vertex_id, int level);
    int get_vertex_level(VertexId vertex_id);
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
    void get_target_vertex_ids(std::vector<VertexId>& target_vertex_ids, VertexId vertex_id) const;
    std::string to_json();
    void clean_map();

   private:
    VertexId edge_id_counter_ = 0;
    VertexId get_new_edge_id() { return edge_id_counter_++; }
    EdgeMap edges_;
};

void print_json(VertexMapManager& vertices_manager, EdgeMapManager& edges_manager);
}  // namespace graph_manager
