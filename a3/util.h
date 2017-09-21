#ifndef UTIL_H
#define UTIL_H

#include "Graph.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>

namespace util {

using namespace gdwg;

template<typename N, typename E>
void assertNodesEq(const Graph<N, E> &graph, const std::vector<N> &expectedNodes) {
    std::stringstream actual, expected;
    auto cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(actual.rdbuf());
    // this is the easiest way I can think of to test your graph, while making sure it still works with printNodes()
    graph.printNodes(); // ENSURE YOU MAKE void Graph::printNodes(ostream& stream = std::cout)
    std::copy(expectedNodes.begin(), expectedNodes.end(), std::ostream_iterator<N>(expected, "\n"));

    std::cout.rdbuf(cout_buf);
    if (expected.str() != actual.str()) {
        std::cout << "Expected:\n" << expected.str() << "\nGot:\n" << actual.str();
        assert(expected.str() == actual.str());
    }
}

template<typename N, typename E>
void assertEdgesEq(const Graph<N, E> &graph, const N &value, std::multimap<N, E> expectedEdges) {
    std::vector<std::pair<N, E>> sortedEdges(expectedEdges.begin(), expectedEdges.end());
    std::stringstream actual, expected;
    auto cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(actual.rdbuf());

    graph.printEdges(value);
    expected << "Edges attached to Node " << value << "\n";
    if (expectedEdges.empty())
        expected << "(null)\n";
    else {
        std::sort(sortedEdges.begin(), sortedEdges.end(), [](const auto &lhs, const auto &rhs) {
            // prioritise by edge (.second), then take node (.first) if required
            if (lhs.second < rhs.second)
                return true;
            else if (rhs.second < lhs.second)
                return false;
            return lhs.first < rhs.first;
        });
    }
    for (const auto &edge : sortedEdges) {
        expected << edge.first << " " << edge.second << "\n";
    }

    std::cout.rdbuf(cout_buf);
    if (expected.str() != actual.str()) {
        std::cout << "Expected:\n" << expected.str() << "\nGot:\n" << actual.str();
        assert(expected.str() == actual.str());
    }
}

// hey, we're not getting marked for style on the tests
#define assertThrows(e) try {e;assert(false);} catch (std::exception&) {}

}

#endif //UTIL_H
