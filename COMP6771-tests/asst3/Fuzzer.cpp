#include <cassert>
#include <deque>
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <type_traits>
#include <utility>

#include <dlfcn.h>
#include <stdlib.h>

#include "Graph.h"

namespace {
    template <typename ...T> using void_t = void;

    template <typename T, typename = void>
    struct is_hashable : std::false_type {};

    template <typename T>
    struct is_hashable<T, void_t<
        decltype(std::declval<std::hash<T>>()(std::declval<T>())),
        decltype(std::declval<T>() == std::declval<T>())
    >> : std::true_type {};

    struct Unhashable {
        using BaseValueType = size_t;

        Unhashable(const BaseValueType& s = 0):
            value(std::make_unique<BaseValueType>(s))
        {}
        virtual ~Unhashable() = default;

        Unhashable(const Unhashable& other):
            value(std::make_unique<BaseValueType>(*other.value))
        {}
        Unhashable& operator=(const Unhashable& other) {
            Unhashable tmp(other);
            using std::swap;
            swap(*this, tmp);
            return *this;
        }

        Unhashable(Unhashable&&) = default;
        Unhashable& operator=(Unhashable&&) = default;

        friend inline bool operator<(const Unhashable& a, const Unhashable& b) noexcept {
            return *a.value < *b.value;
        }

        std::unique_ptr<BaseValueType> value;

        static std::deque<Unhashable> outputBuffer;
        friend inline std::ostream& operator<<(std::ostream& a, const Unhashable& b) {
            outputBuffer.push_back(b);
            return a << *b.value;
        }
    };

    struct Hashable : Unhashable {
        Hashable(const BaseValueType& s = 0):
            Unhashable(s)
        {}
        virtual ~Hashable() = default;

        Hashable(const Hashable& other):
            Unhashable(other)
        {}
        Hashable& operator=(const Hashable& other) {
            static_cast<Unhashable&>(*this) = other;
            return *this;
        }

        Hashable(Hashable&&) = default;
        Hashable& operator=(Hashable&&) = default;

        friend inline bool operator==(const Hashable& a, const Hashable& b) noexcept {
            return *a.value == *b.value;
        }

        static std::deque<Hashable> outputBuffer;
        friend inline std::ostream& operator<<(std::ostream& a, const Hashable& b) {
            outputBuffer.push_back(b);
            return a << *b.value;
        }
    };

    std::deque<Unhashable> Unhashable::outputBuffer;
    std::deque<Hashable> Hashable::outputBuffer;
}

namespace std {
    template <>
    struct hash<Hashable> {
        inline size_t operator()(const Hashable& a) const noexcept {
            return std::hash<Hashable::BaseValueType>()(*a.value);
        }
    };
}

namespace {
    std::mt19937 _rng;
    bool _inFuzzer;
    bool _enableFailingMalloc;

    struct FailingMallocGuard {
        FailingMallocGuard() {_enableFailingMalloc = true;}
        ~FailingMallocGuard() {_enableFailingMalloc = false;}
    };

    template <typename Fn>
    auto _withFailingMalloc(const Fn& f) -> decltype(f()) {
        FailingMallocGuard guard;
        return f();
    }

    template <typename NodeLabel, typename EdgeValue>
    std::set<NodeLabel> _extractGdwgNodes(const gdwg::Graph<NodeLabel, EdgeValue>& graph) {
        std::set<NodeLabel> result;
        std::ofstream nullStream;

        assert(NodeLabel::outputBuffer.size() == 0);
        graph.printNodes(nullStream);
        for (auto& item : NodeLabel::outputBuffer)
            assert(result.insert(std::move(item)).second);
        NodeLabel::outputBuffer.clear();

        return result;
    }

    template <typename NodeLabel, typename EdgeValue>
    std::set<std::pair<NodeLabel, EdgeValue>> _extractGdwgEdges(const gdwg::Graph<NodeLabel, EdgeValue>& graph, const NodeLabel& from) {
        std::set<std::pair<NodeLabel, EdgeValue>> result;
        std::ofstream nullStream;

        assert(NodeLabel::outputBuffer.size() == 0);
        assert(EdgeValue::outputBuffer.size() == 0);

        graph.printEdges(from, nullStream);

        assert(NodeLabel::outputBuffer.size() > 0);
        NodeLabel::outputBuffer.pop_front(); // Get rid of the from node label

        while (NodeLabel::outputBuffer.size() > 0) {
            NodeLabel label(std::move(NodeLabel::outputBuffer.front()));
            NodeLabel::outputBuffer.pop_front();
            assert(EdgeValue::outputBuffer.size() > 0);
            EdgeValue value(std::move(EdgeValue::outputBuffer.front()));
            EdgeValue::outputBuffer.pop_front();

            assert(result.insert(std::make_pair(std::move(label), std::move(value))).second);
        }
        assert(EdgeValue::outputBuffer.size() == 0);

        return result;
    }

    template <typename NodeLabel, typename EdgeValue>
    struct SlowGraph {
        std::set<NodeLabel> nodes;
        std::set<std::tuple<NodeLabel, NodeLabel, EdgeValue>> edges;
    };

    template <typename NodeLabel, typename EdgeValue>
    void _testAddNode(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel label(valueDist(_rng));
        //std::cerr << "Creating " << *label.value << "...";
        if (_withFailingMalloc([&]{return gdwgGraph.addNode(label);})) {
            //std::cerr << "Success\n";
            if (!slowGraph.nodes.insert(std::move(label)).second)
                throw std::runtime_error("Tried to insert existing node into graph");
        } else {
            //std::cerr << "Failure\n";
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testAddEdge(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel from(valueDist(_rng));
        NodeLabel to(valueDist(_rng));
        EdgeValue value(valueDist(_rng));
        bool isSuccessful = false;
        try {
            //std::cerr << "Inserting (" << *from.value << ", " << *to.value << ", " << *value.value << ")...";
            isSuccessful = _withFailingMalloc([&]{return gdwgGraph.addEdge(from, to, value);});
        } catch (const std::runtime_error& e) {
            //std::cerr << "Failure\n";
            if (slowGraph.nodes.count(from) > 0 && slowGraph.nodes.count(to) > 0)
                std::throw_with_nested(std::runtime_error("Got unexpected exception from gdwg::Graph::addEdge()"));
            else
                return;
        }

        if (isSuccessful) {
            //std::cerr << "Success\n";
            if (!slowGraph.edges.insert(std::make_tuple(std::move(from), std::move(to), std::move(value))).second)
               throw std::runtime_error("Unexpected success in inserting existing edge into graph");
        } else {
            //std::cerr << "Failure\n";
            if (slowGraph.edges.count(std::make_tuple(std::move(from), std::move(to), std::move(value))) == 0)
                throw std::runtime_error("Unexpected failure to insert non-existent edge into graph");
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testIsNode(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel label(valueDist(_rng));
        if (_withFailingMalloc([&]{return gdwgGraph.isNode(label);})) {
            if (slowGraph.nodes.count(label) == 0)
                throw std::runtime_error("Unexpected success in node existence check");
        } else {
            if (slowGraph.nodes.count(label) > 0)
                throw std::runtime_error("Unexpected failure in node existence check");
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testIsConnected(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel fromLabel(valueDist(_rng));
        NodeLabel toLabel(valueDist(_rng));
        bool isSuccessful = false;
        try {
            isSuccessful = _withFailingMalloc([&]{return gdwgGraph.isConnected(fromLabel, toLabel);});
        } catch (const std::runtime_error& e) {
            if (slowGraph.nodes.count(fromLabel) > 0 && slowGraph.nodes.count(toLabel) > 0)
                std::throw_with_nested(std::runtime_error("Got unexpected exception from gdwg::Graph::isConnected()"));
            else
                return;
        }

        if (slowGraph.nodes.count(fromLabel) == 0 || slowGraph.nodes.count(toLabel) == 0)
            throw std::runtime_error(std::runtime_error("Unexpected success in checking node connectivity"));

        bool isActuallyConnected = false;
        for (const auto& edge : slowGraph.edges) {
            if (*std::get<0>(edge).value == *fromLabel.value && *std::get<1>(edge).value == *toLabel.value) {
                isActuallyConnected = true;
                break;
            }
        }

        if (isSuccessful != isActuallyConnected)
            throw std::runtime_error("Wrong response from gdwg::Graph::isConnected()");
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testReplace(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel oldLabel(valueDist(_rng));
        NodeLabel newLabel(valueDist(_rng));
        bool isSuccessful = false;
        try {
            //std::cerr << "Replacing " << *oldLabel.value << " with " << *newLabel.value << "...";
            isSuccessful = _withFailingMalloc([&]{return gdwgGraph.replace(oldLabel, newLabel);});
        } catch (const std::runtime_error& e) {
            //std::cerr << "Failure\n";
            if (slowGraph.nodes.count(oldLabel) > 0)
                std::throw_with_nested(std::runtime_error("Got unexpected exception from gdwg::Graph::replace()"));
            else
                return;
        }

        if (isSuccessful) {
            //std::cerr << "Success\n";
            if (!slowGraph.nodes.erase(oldLabel))
                throw std::runtime_error(std::runtime_error("Unexpected success in replacing from a non-existent node"));
            if (!slowGraph.nodes.insert(newLabel).second)
                throw std::runtime_error(std::runtime_error("Unexpected success in replacing to an existent node"));

            for (auto edge = slowGraph.edges.begin(); edge != slowGraph.edges.end(); ) {
                if (*std::get<0>(*edge).value == *oldLabel.value || *std::get<1>(*edge).value == *oldLabel.value) {
                    auto edgeCopy = *edge;
                    edge = slowGraph.edges.erase(edge);

                    if (*std::get<0>(edgeCopy).value == *oldLabel.value)
                        *std::get<0>(edgeCopy).value = *newLabel.value;
                    if (*std::get<1>(edgeCopy).value == *oldLabel.value)
                        *std::get<1>(edgeCopy).value = *newLabel.value;

                    slowGraph.edges.insert(std::move(edgeCopy));
                } else {
                    ++edge;
                }
            }
        } else {
            //std::cerr << "Failure\n";
            if (slowGraph.nodes.count(newLabel) == 0)
                throw std::runtime_error(std::runtime_error("Unexpected failure in replacing to a non-existent node"));
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testMergeReplace(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel oldLabel(valueDist(_rng));
        NodeLabel newLabel(valueDist(_rng));
        try {
            //std::cerr << "Merging " << *oldLabel.value << " with " << *newLabel.value << "...";
            _withFailingMalloc([&]{return gdwgGraph.mergeReplace(oldLabel, newLabel);});
        } catch (const std::runtime_error& e) {
            //std::cerr << "Failure\n";
            if (slowGraph.nodes.count(oldLabel) > 0 && slowGraph.nodes.count(newLabel) > 0)
                std::throw_with_nested(std::runtime_error("Got unexpected exception from gdwg::Graph::mergeReplace()"));
            else
                return;
        }

        //std::cerr << "Success\n";
        if (*oldLabel.value == *newLabel.value)
            return;
        if (!slowGraph.nodes.erase(oldLabel))
            throw std::runtime_error(std::runtime_error("Unexpected success in merging from a non-existent node"));
        if (slowGraph.nodes.count(newLabel) == 0)
            throw std::runtime_error(std::runtime_error("Unexpected success in merging to a non-existent node"));

        for (auto edge = slowGraph.edges.begin(); edge != slowGraph.edges.end(); ) {
            if (*std::get<0>(*edge).value == *oldLabel.value || *std::get<1>(*edge).value == *oldLabel.value) {
                auto edgeCopy = *edge;
                edge = slowGraph.edges.erase(edge);

                if (*std::get<0>(edgeCopy).value == *oldLabel.value)
                    *std::get<0>(edgeCopy).value = *newLabel.value;
                if (*std::get<1>(edgeCopy).value == *oldLabel.value)
                    *std::get<1>(edgeCopy).value = *newLabel.value;

                slowGraph.edges.insert(std::move(edgeCopy));
            } else {
                ++edge;
            }
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testDeleteNode(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel label(valueDist(_rng));
        //std::cerr << "Deleting " << *label.value << "\n";
        _withFailingMalloc([&]{gdwgGraph.deleteNode(label);});
        if (slowGraph.nodes.erase(label)) {
            for (auto edge = slowGraph.edges.begin(); edge != slowGraph.edges.end(); )
                if (*std::get<0>(*edge).value == *label.value || *std::get<1>(*edge).value == *label.value)
                    edge = slowGraph.edges.erase(edge);
                else
                    ++edge;
        }
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testDeleteEdge(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& valueDist
    ) {
        NodeLabel from(valueDist(_rng));
        NodeLabel to(valueDist(_rng));
        EdgeValue value(valueDist(_rng));
        _withFailingMalloc([&]{gdwgGraph.deleteEdge(from, to, value);});
        slowGraph.edges.erase(std::make_tuple(from, to, value));
    }

    template <typename NodeLabel, typename EdgeValue>
    void _testClear(
        gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph,
        SlowGraph<NodeLabel, EdgeValue>& slowGraph,
        std::uniform_int_distribution<Unhashable::BaseValueType>& /*valueDist*/
    ) {
        _withFailingMalloc([&]{gdwgGraph.clear();});
        slowGraph.nodes.clear();
        slowGraph.edges.clear();
    }

    template <typename NodeLabel, typename EdgeValue>
    void _checkEquality(const gdwg::Graph<NodeLabel, EdgeValue>& gdwgGraph, const SlowGraph<NodeLabel, EdgeValue>& slowGraph) {
        auto nodes = _extractGdwgNodes(gdwgGraph);
        if (nodes.size() != slowGraph.nodes.size()){
        	std::cout << nodes.size() << " vs " << slowGraph.nodes.size() << std::endl;    
	gdwgGraph.printNodes();
	std::cout << "---\n ";
	for(const auto i:nodes){
		std::cout << i <<std::endl;
	}
	throw std::runtime_error("GDWG node count mismatch");
	}
	
        for (const auto& node : nodes)
            if (slowGraph.nodes.count(node) == 0)
                throw std::runtime_error("GDWG contains unknown node");

        size_t edgeCount = 0;
        for (const auto& node : nodes) {
            auto edges = _extractGdwgEdges(gdwgGraph, node);
            edgeCount += edges.size();

            for (const auto& edge : edges)
                if (slowGraph.edges.count(std::make_tuple(node, edge.first, edge.second)) == 0)
                    throw std::runtime_error("GDWG contains unknown edge");
        }
        if (edgeCount != slowGraph.edges.size())
            throw std::runtime_error("GDWG edge count mismatch");
    }

    template <typename NodeLabel, typename EdgeValue>
    void _test() {
        constexpr const size_t ITERATIONS = 10000;
        constexpr const bool CHECK_EQUALITY_EVERY_ITERATION = false;

        constexpr const size_t CREATE_PERCENTAGE = 43;
        constexpr const size_t COPY_PERCENTAGE = 5;
        constexpr const size_t QUERY_PERCENTAGE = 10;
        constexpr const size_t MODIFY_PERCENTAGE = 30;
        constexpr const size_t DELETE_PERCENTAGE = 10;
        constexpr const size_t CLEAR_PERCENTAGE = 2;

        static_assert(
            CREATE_PERCENTAGE + COPY_PERCENTAGE + QUERY_PERCENTAGE +
                MODIFY_PERCENTAGE + DELETE_PERCENTAGE + CLEAR_PERCENTAGE == 100,
            "Percentages don't add up to 100"
        );

        struct Graph {
            Graph() = default;

            Graph(const Graph& other) = default;
            Graph& operator=(const Graph& other) {
                Graph tmp(other);
                using std::swap;
                std::swap(*this, tmp);
                return *this;
            }

            Graph(Graph&& other) = default;
            Graph& operator=(Graph&& other) = default;

            gdwg::Graph<NodeLabel, EdgeValue> gdwgGraph;
            SlowGraph<NodeLabel, EdgeValue> slowGraph;
        };
        std::vector<Graph> graphs(1);

        std::uniform_real_distribution<double> valueDistDist(0, 0.5);
        std::uniform_int_distribution<Unhashable::BaseValueType> valueDist(0, ITERATIONS * valueDistDist(_rng));

        std::uniform_int_distribution<size_t> actionDist(0, 99);
        for (size_t n = 0; n < ITERATIONS; ++n) {
            std::uniform_int_distribution<size_t> graphDist(0, graphs.size() - 1);
            size_t graphIndex = graphDist(_rng);
            auto& graph = graphs[graphIndex];

            size_t action = actionDist(_rng);
            try {
                if (action < CREATE_PERCENTAGE) {
                    if (action < CREATE_PERCENTAGE / 4) {
                        action = 0;
                        _testAddNode(graph.gdwgGraph, graph.slowGraph, valueDist);
                    } else {
                        action = 1;
                        _testAddEdge(graph.gdwgGraph, graph.slowGraph, valueDist);
                    }
                    goto next;
                }
                action -= CREATE_PERCENTAGE;

                if (action < COPY_PERCENTAGE) {
                    if (action < COPY_PERCENTAGE / 6) {
                        action = 2;
                        _withFailingMalloc([&]{graphs.emplace_back(graph);});
                    } else if (action < 2 * COPY_PERCENTAGE / 6) {
                        action = 3;
                        graphs.push_back(Graph());
                        _withFailingMalloc([&]{graphs.back() = graphs[graphIndex];});
                    } else if (action < 3 * COPY_PERCENTAGE / 6) {
                        action = 4;
                        _withFailingMalloc([&]{graphs[graphDist(_rng)] = graph;});
                    } else if (action < 4 * COPY_PERCENTAGE / 6) {
                        action = 5;
                        _withFailingMalloc([&]{graphs.emplace_back(std::move(graph));});
                    } else if (action < 5 * COPY_PERCENTAGE / 6) {
                        action = 6;
                        graphs.push_back(Graph());
                        _withFailingMalloc([&]{graphs.back() = std::move(graphs[graphIndex]);});
                    } else {
                        action = 7;
                        size_t otherGraphIndex = graphDist(_rng);
                        if (otherGraphIndex != graphIndex)
                            _withFailingMalloc([&]{graphs[otherGraphIndex] = std::move(graph);});
                    }
                    goto next;
                }
                action -= COPY_PERCENTAGE;

                if (action < QUERY_PERCENTAGE) {
                    if (action < QUERY_PERCENTAGE / 2) {
                        action = 8;
                        _testIsNode(graph.gdwgGraph, graph.slowGraph, valueDist);
                    } else {
                        action = 9;
                        _testIsConnected(graph.gdwgGraph, graph.slowGraph, valueDist);
                    }
                    goto next;
                }
                action -= QUERY_PERCENTAGE;

                if (action < MODIFY_PERCENTAGE) {
                    if (action < MODIFY_PERCENTAGE / 2) {
                        action = 10;
                        _testReplace(graph.gdwgGraph, graph.slowGraph, valueDist);
                    } else {
                        action = 11;
                        _testMergeReplace(graph.gdwgGraph, graph.slowGraph, valueDist);
                    }
                    goto next;
                }
                action -= MODIFY_PERCENTAGE;

                try {
                    _checkEquality(graph.gdwgGraph, graph.slowGraph);
                }  catch (...) {
                    action = 12;
                    throw;
                }

                if (action < DELETE_PERCENTAGE) {
                    if (action < DELETE_PERCENTAGE / 4) {
                        action = 13;
                        _testDeleteNode(graph.gdwgGraph, graph.slowGraph, valueDist);
                    } else {
                        action = 14;
                        _testDeleteEdge(graph.gdwgGraph, graph.slowGraph, valueDist);
                    }
                    goto next;
                }
                action -= DELETE_PERCENTAGE;

                if (action < CLEAR_PERCENTAGE) {
                    action = 15;
                    _testClear(graph.gdwgGraph, graph.slowGraph, valueDist);
                    goto next;
                }

                assert(false);
            } catch (const std::bad_alloc&) {
                // Ignore
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Fuzzer failed on iteration " + std::to_string(n) + " action " + std::to_string(action)));
            }

        next:
            if (CHECK_EQUALITY_EVERY_ITERATION) try {
                for (const auto& graph : graphs)
                    _checkEquality(graph.gdwgGraph, graph.slowGraph);
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Equality check failed on iteration " + std::to_string(n) + " action " + std::to_string(action)));
            }
        }

        for (const auto& graph : graphs)
            _checkEquality(graph.gdwgGraph, graph.slowGraph);
    }

    void _printException(const std::exception& e, size_t level = 0) {
        std::cerr << std::string(level, ' ') << "Exception: " << e.what() << "\n";
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception& e) {
            _printException(e, level + 1);
        } catch (...) {
            std::cerr << std::string(level + 1, ' ') << "Unknown exception\n";
        }
    }
}

void* malloc(size_t size) noexcept {
  //  constexpr const size_t FAILURE_PERCENTAGE = 0; // Set to non-zero for strong exception guarantee testing

    static decltype(malloc)* _malloc;
    static std::uniform_int_distribution<size_t> dist(0, 99);

    if (!_malloc) {
        _malloc = reinterpret_cast<decltype(malloc)*>(dlsym(RTLD_NEXT, "malloc"));
        assert(_malloc);
    }
/*
    if (_enableFailingMalloc && dist(_rng) < FAILURE_PERCENTAGE)
        return nullptr;
*/
    void* result = _malloc(size);
    if (!result && _inFuzzer) {
        std::cerr << "Actual allocation failure while in fuzzer - can't handle this!";
        std::terminate();
    }
    return result;
}

void fuzzer() {
    constexpr const size_t LOOPS = 100;

    auto seed = std::random_device()();
    std::cerr << "Seeding fuzzer with " << seed << "\n";
    _rng.seed(seed);

    _inFuzzer = true;
    try {
        for (size_t n = 0; n < LOOPS; ++n) {
            try {
                _test<Unhashable, Unhashable>();
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Fuzzer failed on loop " + std::to_string(n) + " of Unhashable-Unhashable"));
            }

            try {
                _test<Hashable, Unhashable>();
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Fuzzer failed on loop " + std::to_string(n) + " of Hashable-Unhashable"));
            }

            try {
                _test<Unhashable, Hashable>();
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Fuzzer failed on loop " + std::to_string(n) + " of Unhashable-Hashable"));
            }

            try {
                _test<Hashable, Hashable>();
            } catch (...) {
                std::throw_with_nested(std::runtime_error("Fuzzer failed on loop " + std::to_string(n) + " of Hashable-Hashable"));
            }
        }

        std::cerr << "Passed fuzzer tests\n";
    } catch (const std::exception& e) {
        _printException(e);
    }
    _inFuzzer = false;
}
