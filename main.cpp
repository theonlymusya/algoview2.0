#include <fstream>
#include <iostream>
#include "block_field.hpp"
#include "graph_data_manager.hpp"
#include "graph_info.hpp"
#include "json_traverser.hpp"
#include "logger.hpp"
#include "xml_parser.hpp"

void set_up_logger(char* input_file_name) {
    std::string log_file_name(input_file_name);
    log_file_name += ".log.txt";
    auto& logger = logger::Logger::get_instance();
    logger.set_outdata_flow(log_file_name);
}

int main(int argc, char* argv[]) {
    using namespace rapidjson;
    using namespace algoview_xml_parser;
    using namespace algoview_json_traverser;
    using namespace graph_info;
    using namespace graph_manager;
    using namespace graph;
    using namespace logger;
    std::map<BlockId, Block*> block_map;
    XML_Parser parser;
    auto& logger = Logger::get_instance();
    set_up_logger(argv[1]);
    logger.log_info_msg("--------------------------1ST STEP--------------------------");
    parser.parse(argv[1]);
    const Document& tree = parser.get_DOM_tree();
    GraphInfo graph;
    logger.log_info_msg("--------------------------2nd STEP--------------------------");
    JSON_Traverser::traverse(tree, graph);
    // graph.print_graph();
    BlockTagsInfo& blocks_info = graph.get_blocks();
    const auto& blocks = blocks_info.get_blocks();
    auto& params = graph.get_params();
    VertexMapManager vertices_manager;
    EdgeMapManager edges_manager;
    logger.log_info_msg("--------------------------3rd STEP--------------------------");
    int it = 0;
    for (const auto& block : blocks) {
        Block* new_block_ptr = new Block(block, it);
        (*new_block_ptr).main_cycle(block, params, vertices_manager, edges_manager, block_map);
        block_map[block.id] = new_block_ptr;
        it++;
    }
    for (const auto& block : block_map) {
        delete block.second;
    }
    std::ofstream output_file("output.json");
    output_file << "{" << vertices_manager.to_json() << edges_manager.to_json() << "}" << std::endl;
    // graph_manager::print_json(vertices_manager, edges_manager);
}

// добавить const в print и get методы
// сделать файл с test mode
// возможно сделать методы парсера статиками
// привести в порядок make file
// добавить вывод ошибки подсчёта регулярного выражения
// calc_expr была шаблонной, но не компилировалась, теперь обрабатывает только double(?)
// переписать asserts (?)
// const block_info??
// поправить генерацию json без дуг
