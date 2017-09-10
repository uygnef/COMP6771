//
// Created by yu on 10/09/17.
//

#ifndef A3_GRAPH_H
#define A3_GRAPH_H

#include <memory>
#include <set>
#include <map>

namespace gdwg {

    template <typename N, typename E>
    class Graph{
    private:
        struct Node;
        struct Edge;

        struct Node{
            N val;
            std::set<std::shared_ptr<Edge>> edges;
            Node() = default;
            Node(N val):val{val}{}
        };

        struct Edge{
            std::weak_ptr<Node> dst;
            E weight;

            Edge(std::shared_ptr<Node> destination, E w):weight{w}{ dst = destination;}
            Edge() = delete;
            Edge(N d, E w):weight{w}{ dst = std::make_shared<Node>(dst); }

            ~Edge(){ dst.reset(); }
        };

        std::map<N, std::shared_ptr<Node>> nodes;


    public:

        Graph(): nodes{}{}

        Graph(const Graph &g): nodes{g.nodes}{}

        Graph(Graph &&g): nodes{std::move(g.nodes)}{}

        ~Graph(){
            nodes.clear();
        }

        bool addNode(const N&);

        bool addEdge(const N& src, const N& dst, const E& w);


    };

    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& input_node) {

        //check if node have already in the nodes map
        auto it = nodes.find(input_node);
        if(it != nodes.end()){
            return false;
        }

        nodes.emplace(input_node, std::make_shared<Node>(Node{input_node}));
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w){
        auto src_i = nodes.find(src);
        if(src_i == nodes.end()){
            throw std::runtime_error("add Edge: source node does not exist.");
        }

        auto dst_i = nodes.find(dst);
        if(dst_i == nodes.end()){
            throw std::runtime_error("add Edge: destination node does not exist.");
        }

        //TODO: use more efficiency method to find if edge is already in the edges set.
        //TODO: reload set find????, reload compare???
        for(auto i: src_i->second->edges){
            if(i.get()->dst.lock() == dst_i->second){
                if(w == i->weight){
                    return false;
                }
            }
        }
        //get source node, and add new edge pointer to it's edges map
        src_i->second->edges.insert(std::make_shared<Edge>(Edge{dst_i->second, w}) );
        return true;

    }

#include "Graph.tem"

}

#endif //A3_GRAPH_H
