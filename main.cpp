#include <fstream>
#include <iostream>
#include "block_field.hpp"
#include "graph_data_manager.hpp"
#include "graph_info.hpp"
#include "json_traverser.hpp"
#include "xml_parser.hpp"

int main() {
    using namespace rapidjson;
    using namespace algoview_xml_parser;
    using namespace algoview_json_traverser;
    using namespace graph_info;
    using namespace graph_manager;
    using namespace graph;
    std::map<BlockId, Block*> block_map;
    XML_Parser parser;
    parser.parse();
    const Document& tree = parser.get_DOM_tree();
    GraphInfo graph;
    JSON_Traverser::traverse(tree, graph);
    graph.print_graph();
    std::cerr << "1";
    BlockTagsInfo& blocks_info = graph.get_blocks();
    const auto& blocks = blocks_info.get_blocks();
    auto& params = graph.get_params();
    VertexMapManager vertices_manager;
    EdgeMapManager edges_manager;
    std::cerr << "2";
    int it = 0;
    for (const auto& block : blocks) {
        std::cerr << "-----------------BLOCK " << it << "-----------------" << std::endl;
        std::cerr << "3";
        Block* new_block_ptr = new Block(block, it);
        std::cerr << "4";
        (*new_block_ptr).main_cycle(block, params, vertices_manager, edges_manager, block_map);
        block_map[block.id] = new_block_ptr;
        it++;
        std::cerr << "5";
    }
    for (const auto& block : block_map) {
        delete block.second;
    }
    std::ofstream output_file("output.txt");
    output_file << "{" << vertices_manager.to_json() << edges_manager.to_json() << "}" << std::endl;
    // graph_manager::print_json(vertices_manager, edges_manager);
}

// добавить проверку размерности блока и количества аргументов
// добавить const в print и get методы
// сделать файл с test mode
// возможно сделать методы парсера статиками
// привести в порядок make file
// добавить вывод ошибки подсчёта регулярного выражения
// calc_expr была шаблонной, но не компилировалась, теперь обрабатывает только double(?)
// переписать asserts (?)
// const block_info??
