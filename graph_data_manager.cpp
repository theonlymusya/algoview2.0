#include "graph_data_manager.hpp"
#include <iostream>
#include "logger.hpp"

namespace graph_manager {
using namespace logger;

int get_max(const std::vector<int>& levels) {
    int max_level = 0;
    for (auto level : levels) {
        if (level > max_level)
            max_level = level;
    }
    return max_level;
}

VertexId VertexMapManager::add_vertex(Vertex* vertex) {
    VertexId new_vertex_id = get_new_vertex_id();
    vertices_[new_vertex_id] = vertex;
    return new_vertex_id;
}

void EdgeMapManager::add_edge(const Edge* edge) {
    edges_[get_new_edge_id()] = edge;
}

void EdgeMapManager::get_target_vertex_ids(std::vector<VertexId>& target_vertex_ids, VertexId vertex_id) const {
    for (const auto& edge : edges_) {
        if (edge.second->source_vertex_id == vertex_id)
            target_vertex_ids.push_back(edge.second->target_vertex_id);
    }
}

void VertexMapManager::add_vertex_level(VertexId vertex_id, int level) {
    auto& logger = Logger::get_instance();
    logger.log_info_msg("Vertex id = " + std::to_string(vertex_id) + " level = " + std::to_string(level));
    vertices_[vertex_id]->level = level;
}

void VertexMapManager::add_info(VertexId vertex_id, const std::string& info) {
    vertices_[vertex_id]->info = info;
}

int VertexMapManager::get_vertex_level(VertexId vertex_id) {
    return vertices_[vertex_id]->level;
}

void GraphCharactManager::inc_level_vertex_counter(int level) {
    if (graph_charact_.each_level_vertex_num.size() <= level) {
        graph_charact_.each_level_vertex_num.resize(level + 1);
    }
    graph_charact_.each_level_vertex_num[level]++;
}

void GraphCharactManager::add_critical_lenght(int length) {
    if (length > graph_charact_.critical_length)
        graph_charact_.critical_length = length;
}

void GraphCharactManager::calculate_width() {
    graph_charact_.width = get_max(graph_charact_.each_level_vertex_num);
}

std::string GraphCharactManager::to_json() {
    std::string result_string;
    calculate_width();
    result_string += "\n\t\"characteristics\":\n\t\t{ \"vertex_num\": ";
    result_string += std::to_string(graph_charact_.vertex_num);
    result_string += ", \"edge_num\": ";
    result_string += std::to_string(graph_charact_.edge_num);
    result_string += ", \"critical_length\": ";
    result_string += std::to_string(graph_charact_.critical_length);
    result_string += ", \"width\": ";
    result_string += std::to_string(graph_charact_.width);
    result_string += "},\n";
    return result_string;
}

std::string VertexMapManager::to_json() {
    std::string result_string;
    result_string += "\t\"vertices\": [";
    for (const auto& vertex : vertices_) {
        std::string vertex_string = "\n\t\t{ \"id\": " + std::to_string(vertex.first) + ", \"coordinates\": [" +
                                    std::to_string(vertex.second->i) + ", " + std::to_string(vertex.second->j) + ", " +
                                    std::to_string(vertex.second->k) + "], \"type\": \"" + vertex.second->type + "\"" +
                                    ", \"level\": " + std::to_string(vertex.second->level) + " },";
        result_string += vertex_string;
    }
    if (result_string.back() != '[')
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
    if (result_string.back() != '[')
        result_string.pop_back();
    result_string += "\n\t],\n";
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
