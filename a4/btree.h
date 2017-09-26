//
// Created by yu on 26/09/17.
//


#ifndef A4_BTREE_H
#define A4_BTREE_H

#include <ostream>
#include <iterator>
#include <set>
#include <memory>


using namespace std;

template <typename T>
class btree {
public:
    btree(size_t maxNodeElems = 40):max_size{maxNodeElems}, root{}{};

    btree(const btree<T>&);
    btree(btree<T>&&);
    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);
    friend ostream& operator<< <> (ostream&, const btree<T>&);
// typedef’s for iterators and declarations
// for begin(), end(), rbegin(), rend() and
// cbegin(), cend(), crbegin() and crend() go here
    iterator find(const T&);
    const_iterator find(const T&) const;
    pair<iterator, bool> insert(const T&);
// make sure your destructor does not leak memory
// (if raw pointers are used)
    ~btree();
private:
// your internal representation goes here
    const static size_t max_size;   //max size of a node
    node_set root;

    struct node;
    struct node_set;
    struct node{
        T val;
        shared_ptr<node_set> child;
        node() = delete;
        node(T val, node_set parent, node_set child):val{val}, child{make_shared(child)}{}
    };

    struct node_set{

        struct compare{
            bool operator() (const std::shared_ptr<node>& lhs, const std::shared_ptr<node>& rhs ){
                return lhs.get()->val < rhs.get()->val;
            }
        };

        shared_ptr<node_set> parent;
        set<shared_ptr, compare> nodes;
        shared_ptr<node_set> last_child;

        //TODO: can it be default?
        node_set() = default;

        //TODO: do we need a shared pointer node A as parameter or T type value and construct a new node?
        bool insert(shared_ptr a){
            if(nodes.size() < max_size){
                return nodes.insert(a).second;
            }
            //TODO: if set is full, append it to child
            if(last_child){}
            return false;
        }
    };



};



#endif //A4_BTREE_H
