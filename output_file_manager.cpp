#include "output_file_manager.hpp"
#include "logger.hpp"
namespace output_file_manager {
using namespace logger;
void OutputFileManager::fatal_error_report() {
    auto& logger = Logger::get_instance();
    output_file_ << "{" << empty_graph_charact_str_ << empty_vertices_str_ << empty_edges_str_ << logger.warn_to_json()
                 << logger.err_to_json() << "}" << std::endl;
}
}  // namespace output_file_manager