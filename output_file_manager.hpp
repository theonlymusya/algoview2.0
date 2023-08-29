#pragma once
#include <fstream>

namespace output_file_manager {

class OutputFileManager {
   public:
    static OutputFileManager& get_instance() {
        static OutputFileManager instance;
        return instance;
    }

    void write(std::string string) { output_file_ << string; };

    void fatal_error_report();

   private:
    OutputFileManager() { output_file_.open("output.json"); };
    ~OutputFileManager() { output_file_.close(); };
    std::ofstream output_file_;
    std::string empty_graph_charact_str_ =
        "\n\t\"characteristics\":\n\t\t{ \"vertex_num\": 0, \"edge_num\": 0, \"critical_length\": 0, \"width\": "
        "0},\n";
    std::string empty_vertices_str_ = "\t\"vertices\": [],\n";
    std::string empty_edges_str_ = "\t\"edges\": [],\n";
};

}  // namespace output_file_manager
