#include "graph_data_manager.hpp"
#include <iostream>

namespace graph_manager {
VertexId VertexMapManager::add_vertex(Vertex* vertex) {
    VertexId new_vertex_id = get_new_vertex_id();
    vertices_[new_vertex_id] = vertex;
    return new_vertex_id;
}

void EdgeMapManager::add_edge(const Edge* edge) {
    edges_[get_new_edge_id()] = edge;
}

void VertexMapManager::add_vertex_level(VertexId vertex_id, int level) {
    vertices_[vertex_id]->level = level;
}

int VertexMapManager::get_vertex_level(VertexId vertex_id) {
    return vertices_[vertex_id]->level;
}

std::string VertexMapManager::to_json() {
    std::string result_string;
    result_string += "\n\t\"vertices\": [";
    for (const auto& vertex : vertices_) {
        std::string vertex_string = "\n\t\t{ \"id\": " + std::to_string(vertex.first) + ", \"coordinates\": [" +
                                    std::to_string(vertex.second->i) + ", " + std::to_string(vertex.second->j) + ", " +
                                    std::to_string(vertex.second->k) + "], \"type\": \"" + vertex.second->type + "\"" +
                                    ", \"level\": " + std::to_string(vertex.second->level) + " },";
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

void print_json(VertexMapManager& vertices_manager, EdgeMapManager& edges_manager) {
    std::cout << "{";
    std::cout << vertices_manager.to_json();
    std::cout << edges_manager.to_json();
    std::cout << "}" << std::endl;
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
}  // namespace graph_manager
