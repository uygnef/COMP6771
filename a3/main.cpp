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
    g.print_edge();

    g.replace("12", "45");
    std::cout << " replace:\n";
    g.print_node();
    g.print_edge();


    g.addEdge("23", "45", 15);
    g.addEdge("45", "23", 5);
    g.print_node();
    g.print_edge();

    g.addNode("fengyu");
    g.addEdge("fengyu", "45", 9);
    g.addEdge("fengyu", "23", 9);
    std::cout << "------------------\n";
    g.mergeReplace("45", "23");
    g.print_node();

    std::cout << "\n ------------- delete edge: \n";
   // g.deleteNode("fengyu");
    g.print_edge();
    g.deleteEdge("23", "23", 5);
    g.deleteEdge("23", "23", 5);
    g.deleteEdge("23", "23", 5);

    g.print_node();
    g.print_edge();
    /*
    std::cout<<"delete node: \n";
    g.deleteNode("45");
    g.print_node();
*/
    return 0;
}