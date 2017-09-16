//
// Created by yu on 10/09/17.
//

#ifndef A3_GRAPH_H
#define A3_GRAPH_H
#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <vector>

namespace gdwg {

    template <typename N, typename E>
    class Graph{
    public:
        struct Node;
        struct Edge;
        struct EdgeCompare;

        using Node_ptr = std::weak_ptr<Node>;
        using Edge_ptr = std::weak_ptr<Edge>;

        struct Node{
            N val;
            std::vector<Edge_ptr> out_edges;
            std::vector<Edge_ptr> in_edges;
            Node() = default;
            Node(N val):val{val}{}

            bool is_equal2(const N& a, const N& b) const{
                return !(a < b || b < a);
            }

            bool find_node(N dst){
                for(const auto& i : out_edges){
                    if(!(i.expired()) && is_equal2(i.lock().get()->dst.lock().get()->val, dst)) {
                        return true;
                    }
                }
                return false;
            }

            unsigned long len(){

                for(auto i = out_edges.cbegin(); i != out_edges.cend();){
                    if(i->expired()){
                        out_edges.erase(i);
                    }else{
                        ++i;
                    }
                }
                return out_edges.size();
            }
        };

        struct Edge{
            Node_ptr dst;
            Node_ptr src;
            E weight;

            Edge(Node_ptr source, Node_ptr destination, E w):dst{destination}, src{source}, weight{w}{}

            Edge() = delete;

            ~Edge(){ dst.reset(); }

        };

        struct EdgeCompare{
            /*
             * make edge class comparable.
             * first compare src than dst if all equal compare weight.
             */
            bool operator()(const std::shared_ptr<Edge>& a, const std::shared_ptr<Edge>& b){
                if( (a.get()->src.lock().get()->val < b.get()->src.lock().get()->val
                     || b.get()->src.lock().get()->val < a.get()->src.lock().get()->val)){
                    return a.get()->src.lock().get()->val < b.get()->src.lock().get()->val;
                } else if( (a.get()->dst.lock().get()->val < b.get()->dst.lock().get()->val
                             || b.get()->dst.lock().get()->val < a.get()->dst.lock().get()->val)) {
                    return a.get()->dst.lock().get()->val < b.get()->dst.lock().get()->val;
                }else{
                    return a.get()->weight < b.get()->weight;
                }
            }


            bool operator()(const Edge_ptr& a, const Edge_ptr& b){
                if(a.lock().get()->src.lock().get()->val < b.lock().get()->src.lock().get()->val
                   || b.lock().get()->src.lock().get()->val < a.lock().get()->src.lock().get()->val ) {
                    return a.lock().get()->src.lock().get()->val < b.lock().get()->src.lock().get()->val;
                }else if(a.lock().get()->dst.lock().get()->val < b.lock().get()->dst.lock().get()->val
                         ||b.lock().get()->dst.lock().get()->val < a.lock().get()->dst.lock().get()->val){
                    return a.lock().get()->dst.lock().get()->val < b.lock().get()->dst.lock().get()->val;
                }else{
                    return a.lock().get()->weight < b.lock().get()->weight;
                }
            }
        };

        struct NodeCompare{
            bool operator()(const Node_ptr& a, const Node_ptr& b){
                return a.lock().get()->val < b.lock().get()->val;
            }
        };

        //TODO: O(n) complexity, can I get O(n)?
        Edge_ptr is_in_edges(N src, N dst, E w){
            for(const auto& i: edges){
                if(is_equal3( i.get()->weight , w) && is_equal( i.get()->src.lock().get()->val, src)
                    && is_equal( i.get()->dst.lock().get()->val , dst)){
                    return i;
                }
            }
            return {};
        }

        std::shared_ptr<Node> is_in_nodes(N name) const{
            for(const auto& i:nodes){
                if(is_equal(i.get()->val, name)){
                    return i;
                }
            }
            return {};
        }

        bool is_equal(const N& a, const N& b) const{
            return !(a < b || b < a);
        }

        bool is_equal3(const E& a, const E& b) const{
            return !(a < b || b < a);
        }

        std::set<std::shared_ptr<Node>, NodeCompare> nodes;
        std::set<std::shared_ptr<Edge>, EdgeCompare> edges;

        mutable typename std::set<std::shared_ptr<Node>, NodeCompare>::iterator iter = nodes.begin();

        ~Graph(){
            nodes.clear();
            edges.clear();
        }
    public:

        Graph(): nodes{}, edges{}{}

        Graph(const Graph& g):nodes{}, edges{}{
                if(this != &g){
                    this->clear();
                   for(const auto& i: g.nodes){
                       this->addNode(i->val);
                   }
                    for(const auto& i: g.edges){
                        this->addEdge(i->src.lock().get()->val, i->dst.lock().get()->val, i->weight);
                    }
                }
        }

        Graph(Graph &&g){
            if(this != &g){
                this->clear();
                std::swap(this->nodes, g.nodes);
                std::swap(this->edges, g.edges);
                std::swap(this->iter, g.iter);
            }
        }

        Graph& operator= (Graph&& g){
            if(this != &g){
                this->clear();
                std::swap(this->nodes, g.nodes);
                std::swap(this->edges, g.edges);
                std::swap(this->iter, g.iter);
            }
            return *this;
        }

        Graph& operator= (const Graph& g){
            if(this != &g){
                this->clear();
                for(const auto& i: g.nodes){
                    this->addNode(i->val);
                }

                for(const auto& i:g.edges){
                    this->addEdge(i->src.lock().get()->val, i->dst.lock().get()->val, i->weight);
                }
            }
            return *this;
        }


        bool addNode(const N&);

        bool addEdge(const N& src, const N& dst, const E& w);

        bool replace(const N& oldData, const N& newData);

        void mergeReplace(const N& oldData, const N& newData);

        void deleteNode(const N&) noexcept;

        void deleteEdge(const N& src, const N& dst, const E& w) noexcept;

        void clear() noexcept;

        bool isNode(const N& val) const;

        bool isConnected(const N& src, const N& dst) const;

        void printNodes(std::ostream& os = std::cout) const;

        void printEdges(const N& val, std::ostream& os = std::cout) const;

        void begin() const;

        bool end() const;

        void next() const;

        const N& value() const;

    };


#include "Graph.tem"

}

#endif //A3_GRAPH_H
