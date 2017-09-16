#include "ProvidedTests.h"
#include <cassert>
#include <unordered_set>
#include <vector>
#include "Graph.h"
#include "util.h"

using namespace util;
using namespace std::string_literals;
using namespace gdwg;


void test6 () {
    // make a graph
    gdwg::Graph<unsigned int,std::string> g;

    assert(g.addNode(1));
    assert(g.addNode(2));
    assert(g.addNode(3));

    assert(g.addEdge(1,2,"1->2"));
    assert(g.addEdge(2,3,"2->3"));
    assert(g.addEdge(3,2,"3->2"));

    g.printNodes();
    g.printEdges(1);
    g.printEdges(2);
    g.printEdges(3);
    g.mergeReplace(2,3);
    g.printNodes();
    std::cout << "$0\n";
    g.printEdges(1);
    //assertEdgesEq(g, 1U, {{3U, "1->2"}});
    std::cout << "$1\n";
    assertThrows(g.printEdges(2));
    std::cout << "$2\n";

    assertEdgesEq(g, 3U, {{3U, "2->3"}, {4U, "2->4"}, {3U, "3->2"}, {4U, "3->4"}, {4U, "cs6771"}});
    std::cout << "$3\n";

    assertEdgesEq(g, 4U, {});
}

void test7() {
    gdwg::Graph<std::string,int> g;

    // add this data into the graph
    assert(g.addNode("a"));
    assert(g.addNode("b"));
    assert(g.addNode("c"));
    assert(g.addNode("d"));

    assert(g.addEdge("b","a",3));
    assert(g.addEdge("b","a",5));
    assert(g.addEdge("c","a",3));

    assertNodesEq(g, {"a", "d", "c", "b"});
    assertEdgesEq(g, "b"s, {{"a", 3}, {"a", 5}});
    assertEdgesEq(g, "c"s, {{"a", 3}});

    g.deleteEdge("b","a",5);
    /*
    g.printEdges("b");

    std::cout << "deleting node a" << std::endl;
    g.deleteNode("a");
    g.printEdges("b");
    std::cout << "Graph after node/edge deletion" << std::endl;
    g.printNodes();

    try {
        g.deleteNode("a");
    } catch( const std::exception &ex ) {
        std::cout << "exception caught 1" << std::endl;
    }

    try {
        g.deleteEdge("c","a",5);
    } catch( const std::exception &ex ) {
        std::cout << "exception caught 2" << std::endl;
    }
    */
}

void test8() {
    {
        std::cout << "Testing copy" << std::endl;
        auto gHeap = new gdwg::Graph<std::string,int>{};

        // add this data into the graph
        assert(gHeap->addNode("a"));
        assert(gHeap->addNode("b"));
        assert(gHeap->addNode("c"));
        assert(gHeap->addNode("d"));

        assert(gHeap->addEdge("b","a",3));
        assert(gHeap->addEdge("b","a",5));
        assert(gHeap->addEdge("b","d",4));
        assert(gHeap->addEdge("c","a",3));

        assertNodesEq(*gHeap, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(*gHeap, "b"s, {{"a"s, 3}, {"a"s, 5}, {"d"s, 4}});

        auto gHeapCopy = *gHeap;
        gHeap->deleteNode("a");
        assertNodesEq(*gHeap, {"c"s, "d"s, "b"s});
        assertEdgesEq(*gHeap, "b"s, {{"d"s, 4}});
        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"a"s, 5}, {"d"s, 4}});

        delete gHeap;
        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
    }

    {
        std::cout << "Testing move" << std::endl;
        auto gHeap = new gdwg::Graph<std::string,int>{};

        // add this data into the graph
        assert(gHeap->addNode("a"));
        assert(gHeap->addNode("b"));
        assert(gHeap->addNode("c"));
        assert(gHeap->addNode("d"));

        assert(gHeap->addEdge("b","a",3));
        assert(gHeap->addEdge("b","a",5));
        assert(gHeap->addEdge("b","d",4));
        assert(gHeap->addEdge("c","a",3));

        assertNodesEq(*gHeap, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(*gHeap, "b"s, {{"a"s, 3}, {"d"s, 4}, {"a"s, 5}});

        std::cout << "moving test 2\n";
        auto gHeapCopy = std::move(*gHeap);
        // We shouldn't use a moved-from object, in general.
        // However, here, for testing purposes, we assumed
        // a moved-from graph is an empty graph.
        assertNodesEq(*gHeap, {});
        delete gHeap;

        std::cout << "moving test 3\n";
        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"d"s, 4}, {"a"s, 5}});
    }
}

void test9() {
    {
        std::cout << "Testing copy" << std::endl;
        auto gHeap = new gdwg::Graph<std::string,int>{};

        // add this data into the graph
        assert(gHeap->addNode("a"));
        assert(gHeap->addNode("b"));
        assert(gHeap->addNode("c"));
        assert(gHeap->addNode("d"));

        assert(gHeap->addEdge("b","a",3));
        assert(gHeap->addEdge("b","a",5));
        assert(gHeap->addEdge("c","a",3));

        assertNodesEq(*gHeap, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(*gHeap, "b"s, {{"a"s, 3}, {"a"s, 5}});

        gdwg::Graph<std::string,int> gHeapCopy;
        gHeapCopy.addNode("z");
        assertNodesEq(gHeapCopy, {"z"s});

        gHeapCopy = *gHeap;    // copy assignment
        gHeap->deleteNode("a");
        assertNodesEq(*gHeap, {"b"s, "c"s, "d"s});
        assertEdgesEq(*gHeap, "b"s, {});

        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"a"s, 5}});

        delete gHeap;
        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"a"s, 5}});
    }

    {
        std::cout << "Testing move" << std::endl;
        auto gHeap = new gdwg::Graph<std::string,int>{};

        // add this data into the graph
        assert(gHeap->addNode("a"));
        assert(gHeap->addNode("b"));
        assert(gHeap->addNode("c"));
        assert(gHeap->addNode("d"));

        assert(gHeap->addEdge("b","a",3));
        assert(gHeap->addEdge("b","a",5));
        assert(gHeap->addEdge("c","a",3));

        assertNodesEq(*gHeap, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(*gHeap, "b"s, {{"a"s, 3}, {"a"s, 5}});

        gdwg::Graph<std::string,int> gHeapCopy;
        gHeapCopy.addNode("z");
        assertNodesEq(gHeapCopy, {"z"s});

        gHeapCopy = std::move(*gHeap);    // move assignment

        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"a"s, 5}});

        // We shouldn't use a moved-from object, in general.
        // However, here, for testing purposes, we assumed
        // a moved-from graph is an empty graph.
        assertNodesEq(*gHeap, {});
        delete gHeap;

        assertNodesEq(gHeapCopy, {"a"s, "d"s, "c"s, "b"s});
        assertEdgesEq(gHeapCopy, "b"s, {{"a"s, 3}, {"a"s, 5}});
    }
}

void test10() {
    gdwg::Graph<std::string,int> gCopy;

    // create some data to store as nodes.
    std::string s = "a";
    std::string t = "b";
    std::string u = "c";

    // add this data into the graph
    assert(gCopy.addNode(s));
    assert(gCopy.addNode(t));
    assert(gCopy.addNode(u));

    assert(gCopy.addEdge(u,t,1));
    assert(gCopy.addEdge(u,t,2));

    const auto& constGraph = gCopy;

    assert(constGraph.isNode("a"));
    assert(!constGraph.isConnected("a","b"));
    assert(constGraph.isConnected("c","b"));

    assertNodesEq(constGraph, {"a"s, "b"s, "c"s});
}

void test11() {
    gdwg::Graph<unsigned int,std::string> g;

    assert(g.addNode(1));
    assert(g.addNode(2));
    assert(g.addNode(3));
    assert(g.addNode(4));

    assert(g.addEdge(1,2,"1->2"));
    assert(g.addEdge(2,3,"2->3"));
    assert(g.addEdge(2,4,"2->4"));
    assert(g.addEdge(3,2,"3->2"));
    assert(g.addEdge(3,4,"3->4"));
    assert(g.addEdge(2,4,"cs6771"));
    assert(g.addEdge(3,4,"cs6771"));

    assertEdgesEq(g, 1U, {{2U, "1->2"s}});
    assertEdgesEq(g, 2U, {{3U, "2->3"s}, {4U, "2->4"s}, {4U, "cs6771"s}});
    assertEdgesEq(g, 3U, {{2U, "3->2"s}, {4U, "3->4"s}, {4U, "cs6771"s}});

    g.mergeReplace(2,3);

    assertEdgesEq(g, 1U, {{3U, "1->2"s}});
    assertThrows(g.printEdges(2));
    assertEdgesEq(g, 3U, {{3U, "3->2"s}, {4U, "3->4"s}, {4U, "cs6771"s}, {3U, "2->3"s}, {4U, "2->4"s}});
}

void test12() {
    gdwg::Graph<unsigned int,int> g;

    assert(g.addNode(1));
    assert(g.addNode(2));
    assert(g.addNode(3));
    assert(g.addNode(4));

    assert(g.addEdge(1,2,12));
    assert(g.addEdge(1,3,13));
    assert(g.addEdge(1,4,14));
    assert(g.addEdge(2,1,21));
    assert(g.addEdge(2,3,23));
    assert(g.addEdge(3,1,31));
    assert(g.addEdge(3,4,34));

    assertNodesEq(g, {4, 2, 3, 1});
}

int main(){
    test8();
}