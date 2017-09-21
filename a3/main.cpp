#include "ProvidedTests.h"
#include <cassert>
#include <unordered_set>
#include <vector>
#include "Graph.h"
#include "util.h"

using namespace util;
using namespace std::string_literals;
using namespace gdwg;

#include "ProvidedTests.h"
#include <cassert>
#include <unordered_set>
#include <vector>
#include "Graph.h"
#include "util.h"

using namespace util;
using namespace std::string_literals;
using namespace gdwg;

void test1() {
    Graph<int, int> g;
    // add some nodes to each graph.
    assert(g.addNode(1));
    int i = 2;
    assert(g.addNode(i));
    double d = 3.41;
    assert(g.addNode(static_cast<int>(d)));
    assertNodesEq(g, {1, 2, 3});

    Graph<std::string, double> g2{};
    std::string s = "world";
    assert(g2.addNode(s));
    assert(g2.addNode("Hello"));
    assertNodesEq(g2, {"Hello", "world"});


    Graph <std::shared_ptr<int>, std::string> g3{};
    std::shared_ptr<int> sp = std::make_shared<int>(5);
    assert(g3.addNode(sp));
    assert(g3.addNode(std::make_shared<int>(6)));
    assert(g3.isNode(sp));
}

void test2() {
    // create 3 graphs
    Graph<int, int> g;
    assert(g.addNode(1));
    int i = 2;
    assert(g.addNode(i));
    double d = 3.41;
    assert(g.addNode(static_cast<int>(d)));
    assert(g.addEdge(2, 1, 3));
    int j = 3;
    assert(g.addEdge(i, j, 1));
    assertEdgesEq(g, 2, {{3, 1}, {1, 3}});
    assertNodesEq(g, {1, 3, 2});


    Graph<std::string, double> g2{};
    g2.addNode("Hello");
    std::string s = "world";
    g2.addNode(s);

    g2.addEdge("Hello", "world", d);
    assertEdgesEq(g2, "Hello"s, {{s, d}});
    assertEdgesEq(g2, s, {});

    Graph<std::shared_ptr<int>, std::string> g3{};
    std::shared_ptr<int> sp = std::make_shared<int>(5);
    g3.addNode(sp);
    g3.addNode(std::make_shared<int>(6));
}

void test3() {
    Graph<int, int> g;
    assert(g.addNode(1));
    int i = 2;
    assert(g.addNode(i));
    double d = 3.41;
    assert(g.addNode(static_cast<int>(d)));
    i = 1;
    assert(!g.addNode(i));
    assert(g.addEdge(2, 1, 3));
    int j = 3;
    assert(g.addEdge(i, j, 1));
    assert(!g.addEdge(2, 1, 3));
    // try to add an edge with a different weight
    assert(g.addEdge(2, 1, 2));
    assertThrows(g.addEdge(7, 1, 3));
    assertThrows(g.addEdge(2, 7, 3));

    Graph<std::string, double> g2{};
    g2.addNode("Hello");
    std::string s = "world";
    g2.addNode(s);
    assert(!g2.addNode("Hello"));
    g2.addEdge("Hello", "world", d);
    assert(!g2.isConnected("world", "Hello"));
    assertThrows(g2.isConnected("hello", "pluto"));

    Graph<std::shared_ptr<int>, std::string> g3{};
    std::shared_ptr<int> sp = std::make_shared<int>(5);
    g3.addNode(sp);
    g3.addNode(std::make_shared<int>(6));
    assert(!g3.addNode(sp));
    assert(g3.addNode(std::make_shared<int>(6)));

    assertThrows(g.printEdges(5));
}

void test4() {
    gdwg::Graph<std::string, int> gCopy;

    // create some data to store as nodes.
    std::string s = "a";
    std::string t = "b";
    std::string u = "c";

    // add this data into the graph
    gCopy.addNode(s);
    gCopy.addNode(t);
    gCopy.addNode(u);

    gCopy.addEdge(u, t, 1);
    gCopy.addEdge(u, t, 2);

    assertNodesEq(gCopy, {s, t, u});
    // change the value of t and make sure that the graph still prints out the original value
    t = "d";
    assertNodesEq(gCopy, {s, "b", u});

    gdwg::Graph<std::shared_ptr<std::string>, std::shared_ptr<int>> gPtr;
    std::shared_ptr<std::string> sPtr = std::make_shared<std::string>("a");
    std::shared_ptr<std::string> tPtr = std::make_shared<std::string>("b");
    std::shared_ptr<std::string> uPtr = std::make_shared<std::string>("c");
    gPtr.addNode(sPtr);
    gPtr.addNode(tPtr);
    gPtr.addNode(uPtr);

    // add an edge between u and t
    gPtr.addEdge(uPtr, tPtr, std::make_shared<int>(1));
    // add a second edge between u and t with a different weight
    gPtr.addEdge(uPtr, tPtr, std::make_shared<int>(2));

    // change the value of the data in the ptr
    *tPtr = "d";

    // this should have updated in the graph as well as it is a pointer.
    // can confirm this using isNode
    assert(!gCopy.isNode(t));
    assert(gPtr.isNode(tPtr));
}

void test5() {
    gdwg::Graph<std::string, int> gCopy;

    // create some data to store as nodes.
    std::string s = "a";
    std::string t = "b";
    std::string u = "c";

    // add this data into the graph
    gCopy.addNode(s);
    gCopy.addNode(t);
    gCopy.addNode(u);

    gCopy.addEdge(u, t, 1);
    gCopy.addEdge(u, t, 2);

    assertNodesEq(gCopy, {s, t, u});

    gCopy.replace("a", "e");
    assertNodesEq(gCopy, {t, "e", u});

    assert(!gCopy.replace("b", "c"));

    assertThrows(gCopy.replace("a", "d"));
}

void test6 () {
    // make a graph
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

    assertEdgesEq(g, 1U, {{2U, "1->2"}});
    assertEdgesEq(g, 2U, {{3U, "2->3"}, {4U, "2->4"}, {4U, "cs6771"}});
    assertEdgesEq(g, 3U, {{2U, "3->2"}, {4U, "3->4"}, {4U, "cs6771"}});
    assertEdgesEq(g, 4U, {});

    g.mergeReplace(2,3);

    assertEdgesEq(g, 1U, {{3U, "1->2"}});
    assertThrows(g.printEdges(2));
    assertEdgesEq(g, 3U, {{3U, "2->3"}, {4U, "2->4"}, {3U, "3->2"}, {4U, "3->4"}, {4U, "cs6771"}});
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

void test13() {
    // make a graph
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

    std::unordered_set<unsigned int> nodes;
    for (g.begin(); !g.end(); g.next())
        assert(nodes.insert(g.value()).second);
    for (unsigned int i = 1; i <= 4; ++i)
        assert(nodes.count(i) > 0);

    const auto& cg = g;
    nodes.clear();
    for (cg.begin(); !cg.end(); cg.next())
        assert(nodes.insert(cg.value()).second);
    for (unsigned int i = 1; i <= 4; ++i)
        assert(nodes.count(i) > 0);
}

const std::vector<std::function<void()>> tests = {test1, test2, test3, test4, test5, test6, test7, test8, test9, test10, test11, test12, test13};

void providedTests() {
    for (auto i = 0U; i < tests.size(); i++) {
        std::cout << "Running test " << i + 1 << "..." << std::endl;
        tests[i]();
    }
}


int main(){
   // providedTests();
    gdwg::Graph<unsigned int,int> g;
    std::cout << g.addNode(1);
    std::cout << g.addNode(1);
    g.addEdge(1, 1, 1);
    g.addNode(2);
    g.replace(1, 3);
    g.mergeReplace(3,2);
    g.printNodes();
}