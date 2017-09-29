/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node
 * that stores m client elements partition the tree
 * into m + 1 sorted subtrees.
 */

#ifndef A4_BTREE_H
#define A4_BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <set>
#include <memory>


// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

template <typename T>
class btree {
public:
    friend class btree_iterator<T> ;
    /** Hmm, need some iterator typedefs here... friends? **/

    /**
     * Constructs an empty btree.  Note that
     * the elements stored in your btree must
     * have a well-defined zero-arg constructor,
     * copy constructor, operator=, and destructor.
     * The elements must also know how to order themselves
     * relative to each other by implementing operator<
     * and operator==. (These are already implemented on
     * behalf of all built-ins: ints, doubles, strings, etc.)
     *
     * @param maxNodeElems the maximum number of elements
     *        that can be stored in each B-Tree node
     */
    btree(size_t maxNodeElems = 40):max_size{maxNodeElems}, root{std::make_shared<node_set>(node_set(maxNodeElems))}{};

    /**
     * The copy constructor and  assignment operator.
     * They allow us to pass around B-Trees by value.
     * Although these operations are likely to be expensive
     * they make for an interesting programming exercise.
     * Implement these operations using value semantics and
     * make sure they do not leak memory.
     */

    /**
     * Copy constructor
     * Creates a new B-Tree as a copy of original.
     *
     * @param original a const lvalue reference to a B-Tree object
     */
    btree(const btree<T>& original);

    /**
     * Move constructor
     * Creates a new B-Tree by "stealing" from original.
     *
     * @param original an rvalue reference to a B-Tree object
     */
//    btree(btree<T>&& original);


    /**
     * Copy assignment
     * Replaces the contents of this object with a copy of rhs.
     *
     * @param rhs a const lvalue reference to a B-Tree object
     */
//    btree<T>& operator=(const btree<T>& rhs);

    /**
     * Move assignment
     * Replaces the contents of this object with the "stolen"
     * contents of original.
     *
     * @param rhs a const reference to a B-Tree object
     */
//    btree<T>& operator=(btree<T>&& rhs);

    /**
     * Puts a breadth-first traversal of the B-Tree onto the output
     * stream os. Elements must, in turn, support the output operator.
     * Elements are separated by space. Should not output any newlines.
     *
     * @param os a reference to a C++ output stream
     * @param tree a const reference to a B-Tree object
     * @return a reference to os
     */
//    friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);

    /**
     * The following can go here
     * -- begin()
     * -- end()
     * -- rbegin()
     * -- rend()
     * -- cbegin()
     * -- cend()
     * -- crbegin()
     * -- crend()
     */

    /**
      * Returns an iterator to the matching element, or whatever
      * the non-const end() returns if the element could
      * not be found.
      *
      * @param elem the client element we are trying to match.  The elem,
      *        if an instance of a true class, relies on the operator< and
      *        and operator== methods to compare elem to elements already
      *        in the btree.  You must ensure that your class implements
      *        these things, else code making use of btree<T>::find will
      *        not compile.
      * @return an iterator to the matching element, or whatever the
      *         non-const end() returns if no such match was ever found.
      */
    using iterator = btree_iterator<T>;
//    iterator find(const T& elem);

    /**
      * Identical in functionality to the non-const version of find,
      * save the fact that what's pointed to by the returned iterator
      * is deemed as const and immutable.
      *
      * @param elem the client element we are trying to match.
      * @return an iterator to the matching element, or whatever the
      *         const end() returns if no such match was ever found.
      */
//    const_iterator find(const T& elem) const;

    /**
      * Operation which inserts the specified element
      * into the btree if a matching element isn't already
      * present.  In the event where the element truly needs
      * to be inserted, the size of the btree is effectively
      * increases by one, and the pair that gets returned contains
      * an iterator to the inserted element and true in its first and
      * second fields.
      *
      * If a matching element already exists in the btree, nothing
      * is added at all, and the size of the btree stays the same.  The
      * returned pair still returns an iterator to the matching element, but
      * the second field of the returned pair will store false.  This
      * second value can be checked to after an insertion to decide whether
      * or not the btree got bigger.
      *
      * The insert method makes use of T's zero-arg constructor and
      * operator= method, and if these things aren't available,
      * then the call to btree<T>::insert will not compile.  The implementation
      * also makes use of the class's operator== and operator< as well.
      *
      * @param elem the element to be inserted.
      * @return a pair whose first field is an iterator positioned at
      *         the matching element in the btree, and whose second field
      *         stores true if and only if the element needed to be added
      *         because no matching element was there prior to the insert call.
      */
    std::pair<iterator, bool> insert(const T& elem);
    iterator find(const T& elem);

    /**
      * Disposes of all internal resources, which includes
      * the disposal of any client objects previously
      * inserted using the insert operation.
      * Check that your implementation does not leak memory!
      */
    ~btree() = default;

private:
    iterator end() const;
    size_t max_size;
    // return type of insert function
    enum IS_INSERTABLE{
        FULL = 0,
        DUP = 1,
        SUCCESS = 2
    };

    // The details of your implementation go here
    struct node;
    struct node_set;

    std::shared_ptr<node_set>  copy_nodes(const node_set& origin);


    //const size_t max_size;   //max size of a node
    std::shared_ptr<node_set> root = std::make_shared<node_set>(node_set(max_size));

    struct node{
        T val;
        using Node_ptr = std::shared_ptr<node>;
        std::shared_ptr<node_set> child;

        node() = delete;
        node(T val, std::shared_ptr<node_set> child, size_t max_size):val{val} { child = std::make_shared<node_set>(node_set(max_size));}
        node(const node& orig);
    };

    struct node_set{

        node_set() = delete;
        node_set(size_t a): max_size{a}{std::cout << "init----------"<< max_size << "\n";}

        struct compare{
            bool operator() (const std::shared_ptr<node>& lhs, const std::shared_ptr<node>& rhs ){
                return lhs.get()->val < rhs.get()->val;
            }
        };

        std::shared_ptr<node_set> parent;
        std::set<std::shared_ptr<node>, compare> nodes;
        std::shared_ptr<node_set> last_child = nullptr;
        const size_t max_size;

        //TODO: can it be default?


        node_set(const node_set& origin);
        //TODO: do we need a shared pointer node A as parameter or T type value and construct a new node?
        // insert into nodes set
        std::pair<typename std::set<std::shared_ptr<node>>::iterator,  IS_INSERTABLE> insert(std::shared_ptr<node> a){
            std::cout << "size si:" << max_size << "\n";
            if(nodes.size() < max_size){
                auto result = nodes.insert(a);
                if(result.second){
                    return std::make_pair(result.first, SUCCESS);
                }
                return std::make_pair(result.first, DUP);
            }
            auto pos = nodes.find(a);

            //return the last smaller element's iterator in the nodes set when it's full.
            if(pos == nodes.end()){
                for(auto i = nodes.begin(); i != nodes.end(); ++i){
                    if(a.get()->val < i->get()->val){
                        return std::make_pair(i, FULL);
                    }
                }
                return std::make_pair(pos, FULL);
            }
            return std::make_pair(pos, DUP);
        }

    };

};

template <typename T>
btree<T>::node_set::node_set(const btree<T>::node_set &origin): max_size{origin.max_size} {
    for(const auto& i: origin.nodes){
        auto new_node = std::make_shared<node>(node(i.get()->val, nullptr, origin.max_size));
        if(i.get()->child == nullptr){
            nodes.insert(new_node);
        }else{
            new_node.get()->child = i.get()->child;
            nodes.insert(new_node);
        }
    }
}

template <typename T>
btree<T>::node::node(const btree<T>::node &orig) {
    val = orig.val;
    //TODO: ERROR
    child = orig.child;
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem) {
    auto new_node_pointer = std::make_shared<node>(node{elem, nullptr, max_size});
    auto temp_node_set = root;
    auto inset_flag = temp_node_set.get()->insert(new_node_pointer);
    while( inset_flag.second != SUCCESS ){
        if(inset_flag.second == DUP){
            return std::make_pair(btree_iterator<T>(temp_node_set, inset_flag.first), false);
        }

        if(inset_flag.second == FULL){
            if(inset_flag.first == temp_node_set.get()->nodes.end()){
                if(temp_node_set.get()->last_child == nullptr){
                    temp_node_set.get()->last_child = std::make_shared<node_set>(node_set(this->max_size));
                }
                temp_node_set = temp_node_set.get()->last_child;
            }else{
                if(inset_flag.first->get()->child == nullptr){
                    inset_flag.first->get()->child = std::make_shared<node_set>(node_set(this->max_size));
                }
                temp_node_set = inset_flag.first->get()->child;
            }
        }
        inset_flag = temp_node_set.get()->insert(new_node_pointer);
    }
    return std::make_pair(btree_iterator<T>(temp_node_set, inset_flag.first), true);
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem) {
    auto temp_node_set = root;

    while(true){
        for(auto i = temp_node_set.get()->nodes.cbegin(); i != temp_node_set.get()->nodes.end(); ++i){
            if(elem == i->get()->val){
                return btree_iterator<T>(temp_node_set, i);
            }
            if(elem < i->get()->val){
                if(i->get()->child == nullptr){
                    return end();
                }
                temp_node_set = i->get()->child;
                continue;
            }
        }
        temp_node_set = temp_node_set.get()->last_child;
    }
}

template <typename T>
typename btree<T>::iterator btree<T>::end() const {
    return btree_iterator<T>(nullptr, nullptr);
}

template <typename T>
btree<T>::btree(const btree<T> &original) {
    max_size = original.max_size;
    root = copy_nodes(*original.root);
}


template <typename T>
typename std::shared_ptr<typename btree<T>::node_set> btree<T>::copy_nodes(const btree<T>::node_set &origin) {
    auto ret = std::make_shared<node_set>(node_set(origin.max_size));
    for(const auto& i: origin.nodes){
        if( i.get()->child == nullptr){
            ret.get()->insert(std::make_shared<node>(node(i.get()->val, nullptr, max_size)));
        }else{
            auto new_node = std::make_shared<node>(node(i.get()->val, nullptr, max_size));
            new_node.get()->child = copy_nodes(*i.get()->child.get());
        }
    }
    if(origin.last_child == nullptr){
        ret.get()->last_child = nullptr;
    }else{
        ret.get()->last_child = copy_nodes(*origin.last_child.get());
    }
    return ret;
}


#endif
