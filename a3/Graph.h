//
// Created by yu on 10/09/17.
//

#ifndef A3_GRAPH_H
#define A3_GRAPH_H

#include <memory>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

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

            Edge(Node_ptr source, Node_ptr destination, E w):weight{w}, dst{destination}, src{source}{}

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

        Graph(const Graph &g): nodes{g.nodes}{}

        Graph(Graph &&g): nodes{std::move(g.nodes)}{}

        ~Graph(){
            nodes.clear();
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

        void begin();

        bool end();

        void next();

        const N& value() const;


    };

    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& input_node) {
        return nodes.insert(std::make_shared<Node>(Node{input_node})).second;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w){
        /*
         * create a new node object, try to find it in the nodes set.
         * TODO: avoid create new object, just found value in nodes set..
         *
         */
        auto src_node = std::make_shared<Node>(Node(src));
        auto src_index = nodes.find(src_node);
        if(src_index == nodes.end()){
            throw std::runtime_error("add Edge: source node does not exist.");
        }
        auto dst_node = std::make_shared<Node>(Node(dst));
        auto dst_index = nodes.find(dst_node);
        if(dst_index == nodes.end()){
            throw std::runtime_error("add Edge: destination node does not exist.");
        }

        //check if the new edge is already in the edges sets.
        auto new_edge = std::make_shared<Edge>(Edge{*src_index ,*dst_index, w});
        bool is_unique = edges.insert(new_edge).second;
        if(!is_unique){
            return false;
        }


        auto a = Edge_ptr(new_edge);
        src_index->get()->out_edges.push_back(a);
        dst_index->get()->in_edges.push_back(a);
        return true;
    }



    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData){
        const auto& old_Node = std::make_shared<Node>(Node(oldData));
        const auto& target_node = nodes.find(old_Node);
        if(target_node == nodes.end()){
            throw std::runtime_error("replace: old data node does not exist.");
        }

        const auto& new_Node = std::make_shared<Node>(Node(newData));
        const auto& new_node = nodes.find(new_Node);
        if(new_node != nodes.end()){
            return false;
        }

        /*
         * change the data(value) pointed by old shared pointer
         * copy old pointer to new_pointer.
         * delete old pointer in the nodes set.
         * put new pointer into nodes set.
         */

        std::shared_ptr<Node> new_pointer = *target_node;
        new_pointer.get()->val = newData;
        nodes.erase(target_node);
        nodes.insert(new_pointer);
        return true;
    }

/*
 * For oldNode, insert all it's out edge into new node
 *              change all it's in edge dst to new node
 *              also need to check duplicated edge.
 */
    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N &oldData, const N &newData) {
        const auto& _old_Node = std::make_shared<Node>(Node(oldData));
        const auto& old_node = nodes.find(_old_Node);
        if(old_node == nodes.end()){
            throw std::runtime_error("mergeReplace: old data node does not exist.");
        }

        const auto& _new_node = std::make_shared<Node>(Node(newData));
        const auto& new_node = nodes.find(_new_node);
        if(new_node == nodes.end()){
            throw std::runtime_error("mergeReplace: new data node does not exist.");
        }

        for(const auto &in_edge: old_node->get()->in_edges) {
            const auto& result_pointer = is_in_edges(in_edge.lock().get()->src.lock().get()->val, new_node->get()->val, in_edge.lock().get()->weight).lock();
            if ( result_pointer != nullptr) {
                edges.erase(in_edge.lock());
            } else {
                in_edge.lock().get()->dst = *new_node;
                new_node->get()->in_edges.push_back(in_edge);
            }
        }

        for(const auto &out_edge: old_node->get()->out_edges) {
            const auto& result_pointer = is_in_edges(new_node->get()->val, out_edge.lock().get()->dst.lock().get()->val, out_edge.lock().get()->weight).lock();
            if (result_pointer != nullptr) {
                edges.erase(out_edge.lock());
            } else {
                out_edge.lock().get()->src = *new_node;
                new_node->get()->out_edges.push_back(out_edge);
            }
        }
        nodes.erase(old_node);
    }

    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N &node_name) noexcept {
        const auto& node = is_in_nodes(node_name);

        if(node.get() == nullptr){
            return;
        }

        /*
         * disable all in_edgs(node owns out edgs)
         */

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
            if(i->get()->weight == w && i->get()->dst.lock().get()->val == dst && i->get()->src.lock().get()->val == src){
                edges.erase(i++);
            } else{
                ++i;
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::clear() noexcept {
        edges.clear();
        nodes.clear();
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
    void Graph<N, E>::printNodes() const {
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
            std::cout << i.second << std::endl;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N &val) const {
        const auto& node =  is_in_nodes(val);
        if(node.get() == nullptr){
            throw std::runtime_error("printEdge: node does not exist.");
        }

        using Edge_print = std::pair<E, N>;
        struct compare{
            bool operator()(const Edge_print & a, const Edge_print & b) {
                if(a.first != b.first){
                    return a.first < b.first;
                } else{
                    return a.second < b.second;
                }
            }
        };

        std::set<Edge_print, compare> print_edge;
        std::cout << "Edges attached to Node " << node.get()->val << std::endl;
        for(const auto& i: node.get()->out_edges){
            print_edge.insert(std::make_pair(i.lock().get()->weight, i.lock().get()->dst.lock().get()->val));
        }

        for(const auto& i: print_edge){
            std::cout << i.second << " " << i.first << std::endl;
        }

    }

    template <typename N, typename E>
    void Graph<N, E>::begin() {
        iter = nodes.begin();
    }

    template <typename N, typename E>
    bool Graph<N, E>::end() {
        return iter == nodes.end();
    }

    template <typename N, typename E>
    void Graph<N, E>::next() {
        ++iter;
    }

    template <typename N, typename E>
    const N &Graph<N, E>::value() const {
        return iter->get()->val;
    }


#include "Graph.tem"

}

#endif //A3_GRAPH_H
