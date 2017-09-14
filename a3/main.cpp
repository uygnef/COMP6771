#include <iostream>
#include "Graph.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    auto d = new gdwg::Graph<std::string,int>;
    std::cout << "start:\n";

    auto g = *d;
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
    g.addEdge("fengyu", "fengyu", 2);

    g.printEdges("fengyu");

    std::cout << "\ncall iterator:\n";
    for (g.begin(); !g.end(); g.next())
        std::cout << g.value() << std::endl;

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
    std::cout << "clear:\n";
    g.print_node();
    g.print_edge();
*/
    g.printNodes();
    g.addEdge("fengyu", "fengyu", 3);
    std::cout<<"-------------delete node: \n";
    g.deleteNode("23");
    g.print_node();
    g.print_edge();

    std::cout << "IS SSSSSSSSSSSSS node:\n";
    std::cout << g.isNode("23");
/*
    std::cout<<"-------------clear graph: \n";

    g.clear();
    g.print_edge();
    g.print_node();

*/
    std::cout<<"-------------is connect: \n";
    std::cout << g.isConnected("fengyu", "fengyu");
    g.addNode("23");
    std::cout << g.isConnected("23", "fengyu");
    std::cout << "Print Node: \n";
    g.printNodes();
    g.printEdges("fengyu");

    for (g.begin(); !g.end(); g.next())
        std::cout << g.value() << std::endl;
    return 0;
}