#include <iostream>
#include "Graph.h"
#include <assert.h>

int main() {
    std::cout << "Hello, World!" << std::endl;
    using namespace gdwg;
    auto g = new Graph<std::string, int>{};

    g->addNode("a");
    g->addNode("b");
    g->addNode("c");
    g->addNode("d");

    g->addEdge("b", "a", 3);
    g->addEdge("b", "a", 5);
    g->addEdge("b", "d", 4);
    g->addEdge("c", "a", 3);

    g->printNodes();
    g->printEdges("b");


    std::cout << "\n----\n";
    auto p = std::move(*g);
    g->printNodes();
    std::cout << "\n----\n";
    p.printNodes();
    delete g;
    std::cout << "\n--~~~--\n";
    p.printNodes();
    return 0;
}