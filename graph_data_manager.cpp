#include <iostream>
#include <map>
#include <vector>

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
    void add_vertex(const Vertex* vertex);
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

void VertexMapManager::add_vertex(const Vertex* vertex) {
    vertices_[get_new_vertex_id()] = vertex;
}

void EdgeMapManager::add_edge(const Edge* edge) {
    edges_[get_new_edge_id()] = edge;
}

std::string VertexMapManager::to_json() {
    std::string result_string;
    result_string += "\n\t\"vertices\": [";
    for (const auto& vertex : vertices_) {
        std::string vertex_string = "\n\t\t{ \"id\": " + std::to_string(vertex.first) + ", \"coordinates\": [" +
                                    std::to_string(vertex.second->i) + ", " + std::to_string(vertex.second->j) + ", " +
                                    std::to_string(vertex.second->k) + "], \"type\": \"" + vertex.second->type +
                                    "\" },";
        result_string += vertex_string;
    }
    result_string.pop_back();
    result_string += "\n\t],\n";
    clean_map();
    return result_string;
}

std::string EdgeMapManager::to_json() {
    std::string result_string;
    result_string += "\t\"edges\": [";
    for (const auto& edge : edges_) {
        std::string edge_string = "\n\t\t{ \"id\": " + std::to_string(edge.first) +
                                  ", \"sourceVertexId\": " + std::to_string(edge.second->source_vertex_id) +
                                  ", \"targetVertexId\": " + std::to_string(edge.second->target_vertex_id) +
                                  ", \"type\": \"" + edge.second->type + "\" },";
        result_string += edge_string;
    }
    result_string.pop_back();
    result_string += "\n\t]\n";
    clean_map();
    return result_string;
}

void VertexMapManager::clean_map() {
    for (const auto& vertex : vertices_) {
        delete vertex.second;
    }
}

void EdgeMapManager::clean_map() {
    for (const auto& edge : edges_) {
        delete edge.second;
    }
}

void trial(VertexMapManager& vertices_manager, EdgeMapManager& edges_manager) {
    Vertex* vertex_ptr1 = new Vertex{0, 0, 0, 0, "0"};
    vertices_manager.add_vertex(vertex_ptr1);
    Vertex* vertex_ptr2 = new Vertex{0, 0, 0, 1};
    vertices_manager.add_vertex(vertex_ptr2);
    Vertex* vertex_ptr3 = new Vertex{0, 0, 1, 0};
    vertices_manager.add_vertex(vertex_ptr3);
    Vertex* vertex_ptr4 = new Vertex{1, 0, 1, 0, "1"};
    vertices_manager.add_vertex(vertex_ptr4);
    Vertex* vertex_ptr5 = new Vertex{1, 1, 1, 0, "1"};
    vertices_manager.add_vertex(vertex_ptr5);
    Vertex* vertex_ptr6 = new Vertex{1, 1, 1, 1, "1"};
    vertices_manager.add_vertex(vertex_ptr6);
    Edge* edge_ptr1 = new Edge{0, 1};
    edges_manager.add_edge(edge_ptr1);
    Edge* edge_ptr2 = new Edge{1, 2};
    edges_manager.add_edge(edge_ptr2);
}

void print_json(VertexMapManager& vertices_manager, EdgeMapManager& edges_manager) {
    std::cout << "{";
    std::cout << vertices_manager.to_json();
    std::cout << edges_manager.to_json();
    std::cout << "}" << std::endl;
}

int main() {
    VertexMapManager vertices_manager;
    EdgeMapManager edges_manager;
    trial(vertices_manager, edges_manager);
    print_json(vertices_manager, edges_manager);
    // std::cout << "{" << vertices_manager.to_json() << edges_manager.to_json() << "}" << std::endl;
}
