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
                if( !is_equal(a.get()->src.lock().get()->val, b.get()->src.lock().get()->val)){
                    return a.get()->src.lock().get()->val < b.get()->src.lock().get()->val;
                } else if( !is_equal(a.get()->dst.lock().get()->val, b.get()->dst.lock().get()->val)) {
                    return a.get()->dst.lock().get()->val < b.get()->dst.lock().get()->val;
                }else{
                    return a.get()->weight < b.get()->weight;
                }
            }


            bool operator()(const Edge_ptr& a, const Edge_ptr& b){
                if(!is_equal(a.lock().get()->src.lock().get()->val, b.lock().get()->src.lock().get()->val)){
                    return a.lock().get()->src.lock().get()->val < b.lock().get()->src.lock().get()->val;
                }else if(! is_equal(a.lock().get()->dst.lock().get()->val, b.lock().get()->dst.lock().get()->val)) {
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

        static bool is_equal(const N& a, const N& b){
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

//
// Created by yu on 10/09/17.
//


#include <vector>
#include <bits/shared_ptr.h>

    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& input_node) {
        return nodes.insert(std::make_shared<Node>(Node{input_node})).second;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w){
        /*
         * create a new node object, find if it in the nodes set.
         */
        auto src_node = is_in_nodes(src);
        if(src_node == nullptr){
            throw std::runtime_error("add Edge: source node does not exist.");
        }
        auto dst_node = is_in_nodes(dst);
        if(dst_node == nullptr){
            throw std::runtime_error("add Edge: destination node does not exist.");
        }

        //check if the new edge is already in the edges sets.
        auto new_edge = std::make_shared<Edge>(Edge{src_node ,dst_node, w});
        bool is_unique = edges.insert(new_edge).second;
        if(!is_unique){
            return false;
        }

        auto a = Edge_ptr(new_edge);
        src_node.get()->out_edges.push_back(a);
        dst_node.get()->in_edges.push_back(a);
        return true;
    }



    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData){
        const auto& old_Node = is_in_nodes(oldData);
        if(old_Node == nullptr){
            throw std::runtime_error("replace: old data node does not exist.");
        }

        const auto& new_node = is_in_nodes(newData);
        if(new_node != nullptr){
            return false;
        }

        /*
         * Only change the val of Node object.
         * All pointers keep the same.
         * delete and insert is to keep the correct order in nodes set.
         */
        std::vector<std::shared_ptr<Edge>> update_cache;

        for(auto i = edges.begin(); i != edges.end();){
            if(is_equal(i->get()->src.lock().get()->val, oldData) || is_equal(i->get()->dst.lock().get()->val, oldData)){
                update_cache.push_back(*i);
                edges.erase(i);
            }
                ++i;

        }

        std::shared_ptr<Node> new_pointer = old_Node;

        nodes.erase(old_Node);
        new_pointer.get()->val = newData;
        nodes.insert(new_pointer);

        for(const auto& i: update_cache){
            edges.insert(i);
        }
        return true;
    }

/*
 * For oldNode, insert all it's out edge into new node
 *              change all it's in edge dst to new node
 *              also need to check duplicated edge.
 */
    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N &oldData, const N &newData) {
        /*
         * check if the new node and old node are valid.
         */
        const auto& old_node = is_in_nodes(oldData);
        if(old_node == nullptr){
            throw std::runtime_error("mergeReplace: old data node does not exist.");
        }

        if(is_equal(oldData, newData)){
            return;
        }

        const auto& new_node = is_in_nodes(newData);
        if(new_node == nullptr){
            throw std::runtime_error("mergeReplace: new data node does not exist.");
        }


        for(const auto &in_edge: old_node.get()->in_edges) {
            if(in_edge.expired()){
                continue;
            }
            const auto& result_pointer = is_in_edges(in_edge.lock().get()->src.lock().get()->val, new_node.get()->val, in_edge.lock().get()->weight).lock();
            if ( result_pointer != nullptr) {
                edges.erase(in_edge.lock());
            } else {

                auto in_edge_tmp = in_edge.lock();
                edges.erase(in_edge.lock());

                in_edge_tmp.get()->dst = new_node;
                edges.insert(in_edge_tmp);
                new_node.get()->in_edges.push_back(Edge_ptr(in_edge_tmp));
            }
        }

        for(const auto &out_edge: old_node.get()->out_edges) {
            if(out_edge.expired()){
                continue;
            }
            const auto& result_pointer = is_in_edges(new_node.get()->val, out_edge.lock().get()->dst.lock().get()->val, out_edge.lock().get()->weight).lock();
            if (result_pointer != nullptr) {
                edges.erase(out_edge.lock());
            } else {
                auto out_edge_tmp = out_edge.lock();
                edges.erase(out_edge.lock());

                out_edge_tmp.get()->src = new_node;
                edges.insert(out_edge_tmp);
                new_node.get()->out_edges.push_back(Edge_ptr(out_edge_tmp));
            }
        }

        nodes.erase(old_node);

    }

    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N &node_name) noexcept {
        const auto& node = is_in_nodes(node_name);

        if(node == nullptr){
            return;
        }

        for(const auto& i: node.get()->out_edges){
            if(!i.expired()){
                edges.erase(i.lock());
            }
        }

        for(const auto& i: node.get()->in_edges){
            if(!i.expired()){
                edges.erase(i.lock());
            }
        }
        nodes.erase(node);
    }


    template <typename N, typename E>
    void Graph<N, E>::deleteEdge(const N &src, const N &dst, const E &w)noexcept {
        for(auto i=edges.begin(); i != edges.end();){
            if(is_equal3( i->get()->weight , w) && is_equal( i->get()->dst.lock().get()->val, dst)
               && is_equal(i->get()->src.lock().get()->val, src)){
                edges.erase(i);
                return;
            }
            ++i;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::clear() noexcept {
        edges.clear();
        nodes.clear();
        iter = nodes.begin();
    }

    template <typename N, typename E>
    bool Graph<N, E>::isNode(const N &val) const {
        const auto& ret =  is_in_nodes(val);
        return ret.get() != nullptr;
    }

    template <typename N, typename E>
    bool Graph<N, E>::isConnected(const N &src, const N &dst) const {
        const auto& src_node = is_in_nodes(src);
        if(src_node.get() == nullptr){
            throw std::runtime_error("is Connected: source node does not exist.");
        }

        const auto& dst_node = is_in_nodes(dst);
        if(dst_node.get() == nullptr){
            throw std::runtime_error("is Connected: destination does not exist.");
        }

        return src_node.get()->find_node(dst);
    }

    template <typename N, typename E>
    void Graph<N, E>::printNodes(std::ostream& os) const {
        using Node_print = std::pair<unsigned long, N>;
        struct compare{
            bool operator()(const Node_print& a, const Node_print& b){
                if(a.first == b.first){
                    return a.second < b.second;
                }else{
                    return a.first < b.first;
                }
            }
        };
        std::set<Node_print, compare> orderd_nodes;
        for(const auto& i:nodes){
            orderd_nodes.insert(std::make_pair(i.get()->len(), i.get()->val));
        }
        for(const auto& i: orderd_nodes){
            os << i.second << std::endl;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N &val, std::ostream& os) const {
        const auto& node =  is_in_nodes(val);
        if(node.get() == nullptr){
            throw std::runtime_error("printEdge: node does not exist.");
        }

        using Edge_print = std::pair<E, N>;
        struct compare{
            bool operator()(const Edge_print & a, const Edge_print & b) {
                if(a.first < b.first || b.first < a.first){
                    return a.first < b.first;
                } else{
                    return a.second < b.second;
                }
            }
        };

        std::set<Edge_print, compare> print_edge{};
        os << "Edges attached to Node " << node.get()->val << std::endl;
        for(const auto& i: node.get()->out_edges){
            if(!i.expired()) {
                auto first = i.lock().get()->weight;
                auto ma = i.lock().get()->dst;
                auto second = ma.lock().get()->val;
                auto temp = std::make_pair(first, second);
                print_edge.insert(temp);
            }
        }

        if(print_edge.size() == 0){
            os << "(null)" << std::endl;
        }
        for(const auto& i: print_edge){
            os << i.second << " " << i.first << std::endl;
        }

    }

    template <typename N, typename E>
    void Graph<N, E>::begin() const {
        iter = nodes.begin();
    }

    template <typename N, typename E>
    bool Graph<N, E>::end() const {
        return iter == nodes.end();
    }

    template <typename N, typename E>
    void Graph<N, E>::next() const {
        ++iter;
    }

    template <typename N, typename E>
    const N &Graph<N, E>::value() const {
        return iter->get()->val;
    }

//#include "Graph.tem"

}

#endif //A3_GRAPH_H
