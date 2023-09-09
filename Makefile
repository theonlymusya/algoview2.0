FLAGS = -g -std=c++17
all: algoview

algoview: main_o xml_parser_o json_traverser_o argument_o vertex_o block_o graph_info_o expr_o graph_data_manager_o block_field_o logger_o output_file_manager_o
	g++ main.o xml_parser.o json_traverser.o argument.o vertex.o block.o graph_info.o expr.o graph_data_manager.o block_field.o logger.o output_file_manager.o $(FLAGS) -o main

main_o: main.cpp
	g++ -c main.cpp $(FLAGS)

xml_parser_o: xml_parser.cpp
	g++ -c xml_parser.cpp $(FLAGS)

json_traverser_o: json_traverser.cpp
	g++ -c json_traverser.cpp $(FLAGS)

argument_o: argument.cpp
	g++ -c argument.cpp $(FLAGS)

vertex_o: vertex.cpp
	g++ -c vertex.cpp $(FLAGS)

block_o: block.cpp
	g++ -c block.cpp $(FLAGS)

graph_info_o: graph_info.cpp
	g++ -c graph_info.cpp $(FLAGS)

expr_o: expr.cpp
	g++ -c expr.cpp $(FLAGS)

graph_data_manager_o: graph_data_manager.cpp
	g++ -c graph_data_manager.cpp $(FLAGS)

block_field_o: block_field.cpp
	g++ -c block_field.cpp $(FLAGS)

logger_o: logger.cpp
	g++ -c logger.cpp $(FLAGS)

output_file_manager_o: output_file_manager.cpp
	g++ -c output_file_manager.cpp $(FLAGS)

clean:
	rm -rf *.o main