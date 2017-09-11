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

        using Node_ptr = std::shared_ptr<Node>;
        using Edge_ptr = std::shared_ptr<Edge>;

        struct Node{
            N val;
            std::set<Edge_ptr> out_edges;
            std::vector<std::weak_ptr<Edge>> in_edges;
            Node() = default;
            Node(N val):val{val}{}

            /*
             * check if this node have target edge in in_edges set
             */

            //TODO: use more efficiency method to find if edge is already in the edges set.
            //TODO: reload set find????, reload compare???
            bool if_have_edge(const Node_ptr& dst, const E& weight){
                for(const auto &i: out_edges){
                    if(i->weight == weight && i->dst.lock().get() == dst.get()){
                        return true;
                    }
                }
                return false;
            }

        };

        struct Edge{
            std::weak_ptr<Node> dst;
            E weight;

            Edge(Node_ptr destination, E w):weight{w}{
                dst = destination;
            }

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

        void print_node(){
            for(auto i: nodes){
                std::cout <<"node is: " << i.second.get()->val<<std::endl;
                for (const auto& j:i.second.get()->out_edges){
                    std::cout <<"   edge is: " << j.get()->weight;
                }
                std::cout<<std::endl;
            }
        }
        bool addNode(const N&);

        bool addEdge(const N& src, const N& dst, const E& w);

        bool replace(const N& oldData, const N& newData);

        void mergeReplace(const N& oldData, const N& newData);

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
        auto src_node_pair = nodes.find(src);
        if(src_node_pair == nodes.end()){
            throw std::runtime_error("add Edge: source node does not exist.");
        }

        auto dst_node_pair = nodes.find(dst);
        if(dst_node_pair == nodes.end()){
            throw std::runtime_error("add Edge: destination node does not exist.");
        }


        std::cout << "chekc is ------------\n";
        if(src_node_pair->second->if_have_edge(dst_node_pair->second, w)){
            return false;
        }

        //get source node, and add new edge pointer to it's edges map
        Edge_ptr new_edge = std::make_shared<Edge>(Edge{dst_node_pair->second, w});

        src_node_pair->second->out_edges.insert(new_edge);
        dst_node_pair->second->in_edges.push_back(std::weak_ptr<Edge>(new_edge) );
        return true;
    }


    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData){
        auto target_node = nodes.find(oldData);
        if(target_node == nodes.end()){
            throw std::runtime_error("replace: old data node does not exist.");
        }

        const auto& new_node = nodes.find(newData);
        if(new_node != nodes.end()){
            return false;
        }

        /*
         *  old data  |key: old data |shared pointer to old Node|
         *                                     ↓
         *  new node  |key: new data |                  old Node|
         * add a new pair to nodes map. using the old data shared pointer as value
         * and new data as key.
         */
        target_node->second->val = newData;
        nodes.insert(std::make_pair(newData, target_node->second));
        nodes.erase(target_node);
        return true;
    }


    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N &oldData, const N &newData) {

        const auto& old_node_pair = nodes.find(oldData);
        if(old_node_pair == nodes.end()){
            throw std::runtime_error("mergeReplace: old data node does not exist.");
        }
        const auto& new_node_pair = nodes.find(newData);
        if(new_node_pair == nodes.end()){
            throw std::runtime_error("mergeReplace: new data node does not exist.");
        }

        for(const auto &i: old_node_pair->second->edges){
            if(i->dst.lock()->val != oldData){
               // new_node_pair->second->edges.insert(std::move(i->dst));
            }else{

            }
        }

        old_node_pair->second = new_node_pair->second;
        nodes.erase(old_node_pair);
    }


#include "Graph.tem"

}

#endif //A3_GRAPH_H
