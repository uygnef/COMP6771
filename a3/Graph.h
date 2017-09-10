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

            //add compare rule for set
            struct Compare{
                bool operator()(const std::shared_ptr<Edge>& a, const std::shared_ptr<Edge>& b)const{
                    if (a->dst.lock()->val == b->dst.lock()->val){
                        return a->weight < b->weight;
                    }
                    return a->dst.lock()->val < b->dst.lock()->val;
                }
            };
            std::set<std::shared_ptr<Edge>, Compare> edges;

            bool find_dst(N n_val, E e_weight){
                for(auto i=edges.begin(); i != edges.end(); ++i){
                    if(i->get()->dst.lock()->val == n_val){
                        if(i->get()->weight){
                            return true;
                        }
                    }
                }
                return false;
            }

            Node() = default;

            Node(N val):val{val}{}
        };

        struct Edge{
            std::weak_ptr<Node> dst;
            E weight;

            Edge(std::shared_ptr<Node> destination, E w):weight{w}{
                dst = destination;
            }

            Edge() = default;

            ~Edge(){
                dst.reset();
            }
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

        auto dst_node = src_i.second.get();
        if(dst_node)
    }

#include "Graph.tem"

}

#endif //A3_GRAPH_H
