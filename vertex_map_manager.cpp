#include <iostream>
#include <map>
#include <vector>

using VertexId = int;

struct Vertex {
    int block_id;
    int i, j, k;
    std::string type = "0";
};

using VertexMap = std::map<int, const Vertex*>;
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

void VertexMapManager::add_vertex(const Vertex* vertex) {
    vertices_[get_new_vertex_id()] = vertex;
}

std::string VertexMapManager::to_json() {
    std::string result_string;
    result_string += "{\n\t\"vertices\": [\n";
    for (const auto& vertex : vertices_) {
        std::string vertex_string =
            "\t\t{ \"id\": " + std::to_string(vertex.first) + ", \"coordinates\": [" +
            std::to_string((int)vertex.second->i) + ", " + std::to_string((int)vertex.second->j) + ", " +
            std::to_string((int)vertex.second->k) + "], \"type\": \"" + vertex.second->type + "\" },\n";
        result_string += vertex_string;
    }
    result_string.pop_back();
    result_string += "\t]\n}\n";
    clean_map();
    return result_string;
}

void VertexMapManager::clean_map() {
    for (const auto& vertex : vertices_) {
        delete vertex.second;
    }
}

void trial(VertexMapManager& vertex_manager) {
    Vertex* vertex_ptr1 = new Vertex{0, 0, 0, 0, "0"};
    vertex_manager.add_vertex(vertex_ptr1);
    Vertex* vertex_ptr2 = new Vertex{0, 0, 0, 1};
    vertex_manager.add_vertex(vertex_ptr2);
    Vertex* vertex_ptr3 = new Vertex{0, 0, 1, 0};
    vertex_manager.add_vertex(vertex_ptr3);
    Vertex* vertex_ptr4 = new Vertex{1, 0, 1, 0, "1"};
    vertex_manager.add_vertex(vertex_ptr4);
    Vertex* vertex_ptr5 = new Vertex{1, 1, 1, 0, "1"};
    vertex_manager.add_vertex(vertex_ptr5);
    Vertex* vertex_ptr6 = new Vertex{1, 1, 1, 1, "1"};
    vertex_manager.add_vertex(vertex_ptr6);
}

int main() {
    VertexMapManager vertex_manager;
    trial(vertex_manager);
    std::cout << vertex_manager.to_json();
}