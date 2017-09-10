#include <iostream>
#include "Graph.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    gdwg::Graph<std::string,int> g;
    g.addNode("12");
    g.addNode("23");
    std::cout<<g.addEdge("12", "23", 12)<<std::endl;
    std::cout<<g.addEdge("12", "23", 12)<<std::endl;

    return 0;
}