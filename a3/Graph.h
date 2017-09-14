//
// Created by yu on 10/09/17.
//

#ifndef A3_GRAPH_H
#define A3_GRAPH_H
#pragma once

#include <memory>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace gdwg {

    template <typename N, typename E>
    class Graph{


    private:
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

            bool find_node(N dst){
                for(auto i = out_edges.begin(); i != out_edges.end();){
                    if(i->expired()) {
                        out_edges.erase(i);
                    }else if(i->lock().get()->dst.lock().get()->val == dst){
                        return true;
                    }
                    ++i;
                }
                for(auto i = in_edges.begin(); i != in_edges.end();){
                    if(i->expired()) {
                        out_edges.erase(i);
                    }else if(i->lock().get()->src.lock().get()->val == dst){
                        return true;
                    }
                    ++i;
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
             * make edge class compareble.
             * first compare src than dst if all equal compare weight.
             */
            bool operator()(const std::shared_ptr<Edge>& a, const std::shared_ptr<Edge>& b){
                if(a.get()->src.lock().get()->val != b.get()->src.lock().get()->val){
                    return a.get()->src.lock().get()->val < b.get()->src.lock().get()->val;
                } else if(a.get()->dst.lock().get()->val != b.get()->dst.lock().get()->val) {
                    return a.get()->dst.lock().get()->val < b.get()->dst.lock().get()->val;
                }else{
                    return a.get()->weight < b.get()->weight;
                }
            }


            bool operator()(const Edge_ptr& a, const Edge_ptr& b){
                if(a.lock().get()->src.lock().get()->val != b.lock().get()->src.lock().get()->val) {
                    return a.lock().get()->src.lock().get()->val < b.lock().get()->src.lock().get()->val;
                }else if(a.lock().get()->dst.lock().get()->val != b.lock().get()->dst.lock().get()->val){
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
                if( i.get()->weight == w && i.get()->src.lock().get()->val == src
                    && i.get()->dst.lock().get()->val == dst){
                    std::cout << i.get()->weight <<"==" << w <<" ---"<<i.get()->src.lock().get()->val <<" == "<<src;
                    return i;
                }
            }
            return {};
        }

        std::shared_ptr<Node> is_in_nodes(N name) const{
            for(const auto& i:nodes){
                if(i.get()->val == name){
                    return i;
                }
            }
            return {};
        }

        std::set<std::shared_ptr<Node>, NodeCompare> nodes;
        std::set<std::shared_ptr<Edge>, EdgeCompare> edges;

        mutable typename std::set<std::shared_ptr<Node>, NodeCompare>::iterator iter = nodes.begin();


    public:

        Graph(): nodes{}{}

        Graph(const Graph &g): nodes{g.nodes}, edges{g.edges}{}

        Graph(Graph &&g): nodes{std::move(g.nodes)}, edges{std::move(g.edges)}{}

        Graph& operator=(const Graph &g){
            if(this != &g){
                this->nodes = std::move(g.nodes);
                this->edges = std::move(g.edges);
            }
            return *this;
        }
        ~Graph(){
            nodes.clear();
            edges.clear();
        }

        void print_edge(){
            for(auto i:edges){
                std::cout << i.get()->src.lock().get()->val << " " << i.get()->dst.lock().get()->val << " " <<i.get()->weight << std::endl;
            }
        }

        void print_node(){
            //TODO: delete element
            for(auto i: nodes){
                std::cout <<"node is: " << i.get()->val<<std::endl;
                for (const auto j:i.get()->out_edges){
                    if(!j.expired()){
                        std::cout <<"   out edge is: " << j.lock().get()->weight;
                    }
                }
                for (const auto& j:i.get()->in_edges){
                    if(!j.expired()){
                        std::cout <<"   in edge is: " << j.lock().get()->weight;
                    }
                }
                std::cout<<std::endl;
            }
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

        void printNodes() const;

        void printEdges(const N& val) const;

        void begin() const;

        bool end() const;

        void next() const;

        const N& value() const;


    };




#include "Graph.tem"

}

#endif //A3_GRAPH_H
