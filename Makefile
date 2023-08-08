FLAGS = -g -std=c++17
all: algoview

algoview: main.o xml_parser.o json_traverser.o argument.o vertex.o block.o graph_info.o expr.o graph_data_manager.o block_field.o logger.o
	g++ main.o xml_parser.o json_traverser.o argument.o vertex.o block.o graph_info.o expr.o graph_data_manager.o block_field.o logger.o $(FLAGS) -o main

main.o: main.cpp
	g++ -c main.cpp $(FLAGS)

xml_parser.o: xml_parser.cpp
	g++ -c xml_parser.cpp $(FLAGS)

json_traverser.o: json_traverser.cpp
	g++ -c json_traverser.cpp $(FLAGS)

argument.o: argument.cpp
	g++ -c argument.cpp $(FLAGS)

vertex.o: vertex.cpp
	g++ -c vertex.cpp $(FLAGS)

block.o: block.cpp
	g++ -c block.cpp $(FLAGS)

graph_info.o: graph_info.cpp
	g++ -c graph_info.cpp $(FLAGS)

expr.o: expr.cpp
	g++ -c expr.cpp $(FLAGS)

graph_data_manager.o: graph_data_manager.cpp
	g++ -c graph_data_manager.cpp $(FLAGS)

block_field.o: block_field.cpp
	g++ -c block_field.cpp $(FLAGS)

logger.o: logger.cpp
	g++ -c logger.cpp $(FLAGS)

clean:
	rm -rf *.o main