//
// Created by yu on 10/09/17.
//

#ifndef A3_GRAPH_H
#define A3_GRAPH_H

#include <memory>
#include <set>
#include <map>
#include <vector>

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
            std::set<Edge_ptr, EdgeCompare> out_edges;
            std::set<Edge_ptr, EdgeCompare> in_edges;
            Node() = default;
            Node(N val):val{val}{}

            /*
             * check if this node have target edge in in_edges set
             */

            //TODO: use more efficiency method to find if edge is already in the edges set.
            //TODO: reload set find????, reload compare???
//            bool if_have_out_edge(const Node_ptr& dst, const E& weight){
//                for(const auto &i: out_edges){
//                    if(i->weight == weight && i->dst.lock().get() == dst.get()){
//                        return true;
//                    }
//                }
//                return false;
//            }

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

        std::set<std::shared_ptr<Node>, NodeCompare> nodes;
        std::set<std::shared_ptr<Edge>, EdgeCompare> edges;


    public:

        Graph(): nodes{}{}

        Graph(const Graph &g): nodes{g.nodes}{}

        Graph(Graph &&g): nodes{std::move(g.nodes)}{}

        ~Graph(){
            nodes.clear();
        }

        void print_node(){
            for(auto i: nodes){
                std::cout <<"node is: " << i.get()->val<<std::endl;
                for (const auto& j:i.get()->out_edges){
                    std::cout <<"   edge is: " << j.lock().get()->weight;
                }
                std::cout<<std::endl;
            }
        }
        bool addNode(const N&);

        bool addEdge(const N& src, const N& dst, const E& w);

        bool replace(const N& oldData, const N& newData);

        void mergeReplace(const N& oldData, const N& newData);

        void deleteNode(const N&) noexcept;

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
        auto new_edge = std::make_shared<Edge>(Edge{*dst_index ,*dst_index, w});
        bool is_unique = edges.insert(new_edge).second;
        if(!is_unique){
            return false;
        }

        src_index->get()->out_edges.insert(Edge_ptr(new_edge));
        dst_index->get()->in_edges.insert(Edge_ptr(new_edge));
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

        for(auto &in_edge: old_node->get()->in_edges){
            //find if changed edge is duplicated in new node.
            std::shared_ptr<Edge> temp_edge = std::make_shared<Edge>(Edge(in_edge.lock().get()->src, *new_node, in_edge.lock().get()->weight));
            const auto& is_in = edges.find(temp_edge);
            // TODO:
            
            if(is_in != edges.end()){
                in_edge.lock().get()->src = ;
            }
            auto old_src_node = in_edge.lock()->src;
            /* check if src node which have edge to old node already have this edge with same weight pointed to new node.
               if not change the dst node of this edge to new node.
             */
            if(!old_src_node.lock()->if_have_out_edge(new_node, in_edge.lock()->weight)){
                in_edge.lock()->dst = new_node;
                new_node->in_edges.push_back(in_edge);
            }
        }

        for(auto &out_edge: old_node->out_edges){
            auto& dst_from_old_node = out_edge.get()->dst;
            if( !dst_from_old_node.lock()->if_have_out_edge(new_node, out_edge->weight)){
                new_node->out_edges.insert(out_edge);
            }
        }

        nodes.erase(old_node_pair);
    }

    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N &node_name) noexcept {
        const auto& delete_node = nodes.find(node_name);
        if(delete_node == nodes.end()){
            return;
        }
        /*
         * disable all in_edgs(node owns out edgs)
         */
        const auto& node = delete_node->second;
        for(auto& in_edge: node->in_edges){

        }
        nodes.erase(delete_node);
    }


#include "Graph.tem"

}

#endif //A3_GRAPH_H
