FLAGS = -g -std=c++17
all: algoview

algoview: main.o xml_parser.o json_traverser.o argument.o vertex.o block.o graph_info.o expr.o graph_data_manager.o block_field.o
	g++ main.o xml_parser.o json_traverser.o argument.o vertex.o block.o graph_info.o expr.o graph_data_manager.o block_field.o $(FLAGS) -o main

main.o: main.cpp
	g++ -c main.cpp

xml_parser.o: xml_parser.cpp
	g++ -c xml_parser.cpp

json_traverser.o: json_traverser.cpp
	g++ -c json_traverser.cpp

argument.o: argument.cpp
	g++ -c argument.cpp

vertex.o: vertex.cpp
	g++ -c vertex.cpp

block.o: block.cpp
	g++ -c block.cpp

graph_info.o: graph_info.cpp
	g++ -c graph_info.cpp

expr.o: expr.cpp
	g++ -c expr.cpp

graph_data_manager.o: graph_data_manager.cpp
	g++ -c graph_data_manager.cpp

block_field.o: block_field.cpp
	g++ -c block_field.cpp

clean:
	rm -rf *.o main