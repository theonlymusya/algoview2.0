#include "graph_info.hpp"
#include "json_traverser.hpp"
#include "xml_parser.hpp"

int main() {
    using namespace rapidjson;
    using namespace algoview_xml_parser;
    using namespace algoview_json_traverser;
    using namespace graph_info;
    XML_Parser parser;
    parser.parse();
    const Document& tree = parser.get_DOM_tree();
    GraphInfo graph;
    JSON_Traverser::traverse(tree, graph);
    graph.print_graph();
}

// добавить проверку размерности блока и количества аргументов
// добавить const в print и get методы
// сделать файл с test mode
// возможно сделать методы парсера статиками
// привести в порядок make file
