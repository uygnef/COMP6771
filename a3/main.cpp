#include <iostream>
#include "Graph.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    gdwg::Graph<std::string,int> g;
    std::cout << "start:\n";
    g.print_node();

    g.addNode("12");
    std::cout << " add 12:\n";
    g.print_node();

    g.addNode("23");
    std::cout << " add 23:\n";
    g.print_node();

    std::cout<<g.addEdge("12", "23", 12)<<std::endl;
    std::cout<<g.addEdge("12", "23", 11)<<std::endl;
    std::cout<<g.addEdge("12", "23", 11)<<std::endl;
    std::cout << " add 2 edge:\n";

    g.print_node();

    g.replace("12", "45");
    std::cout << " replace:\n";
    g.print_node();

    g.addEdge("23", "45", 15);
    g.addEdge("45", "23", 5);
    g.print_node();
/*
    std::cout << "------------------\n";
    g.mergeReplace("45", "23");
    g.print_node();
*/
    /*
    std::cout<<"delete node: \n";
    g.deleteNode("45");
    g.print_node();
*/
    return 0;
}