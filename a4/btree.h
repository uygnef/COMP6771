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
#include <list>
// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

template <typename T>
class btree {
public:

    friend class btree_iterator<T> ;
    friend class const_btree_iterator<T>;
    using iterator = btree_iterator<T>;

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
    btree(size_t maxNodeElems = 40):max_size{maxNodeElems}, root{std::make_shared<node_list>(node_list(maxNodeElems))}{};

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
    btree(btree<T>&& original);


    /**
     * Copy assignment
     * Replaces the contents of this object with a copy of rhs.
     *
     * @param rhs a const lvalue reference to a B-Tree object
     */
    btree<T>& operator=(const btree<T>& rhs);

    /**
     * Move assignment
     * Replaces the contents of this object with the "stolen"
     * contents of original.
     *
     * @param rhs a const reference to a B-Tree object
     */
    btree<T>& operator=(btree<T>&& rhs);

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

    iterator begin();
    iterator end();

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
    iterator find(const T& elem);

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

    /**
      * Disposes of all internal resources, which includes
      * the disposal of any client objects previously
      * inserted using the insert operation.
      * Check that your implementation does not leak memory!
      */
    ~btree() = default;
    iterator end() const;

private:
    size_t max_size;
    // return type of insert function
    enum IS_INSERTABLE{
        FULL = 0,
        DUP = 1,
        SUCCESS = 2
    };

    // The details of your implementation go here
    struct node;
    struct node_list;

    using set_iter = typename std::list<std::shared_ptr<node>>::iterator;

    std::shared_ptr<node_list> copy_nodes(const node_list& origin);


    //const size_t max_size;   //max size of a node
    std::shared_ptr<node_list> root = std::make_shared<node_list>(node_list(max_size));

    struct node{
        T val;
        using Node_ptr = std::shared_ptr<node>;
        std::shared_ptr<node_list> child;

        node() = delete;
        node(size_t max_size){child = std::make_shared<node_list>(node_list(max_size));}
        node(T val, std::shared_ptr<node_list> child, size_t max_size):val{val} { child = std::make_shared<node_list>(node_list(max_size));}
        node(const node& orig);
    };

    struct node_list{


        node_list() = delete;
        node_list(size_t a): max_size{a}{std::cout << "insdf-------"<< max_size << "\n";}
        node_list(size_t a, btree_iterator<T> p): max_size{a}, parent{p}{std::cout << "init----------"<< max_size << "\n";}

        btree_iterator<T> parent;

        std::list<std::shared_ptr<node>> nodes;
        size_t max_size;

        //TODO: can it be default?


        node_list(const node_list& origin);
        //TODO: do we need a shared pointer node A as parameter or T type value and construct a new node?
        // insert into nodes set
        std::pair<typename std::list<std::shared_ptr<node>>::iterator,  IS_INSERTABLE> insert(std::shared_ptr<node> a){
            // insert into new nodes list
            if(nodes.size() == 0){
                nodes.push_back(a);
                return std::make_pair(nodes.begin(), SUCCESS);
            }

            auto last_element = nodes.end();
            --last_element;
            for(auto i = nodes.begin(); i != nodes.end(); ++i){
                // last elements is a null val and child pointer
                if(i == last_element && max_size < nodes.size() ){
                    return std::make_pair(nodes.end(), FULL);
                }

                if (a.get()->val == i->get()->val) {
                    return std::make_pair(i, DUP);
                }
                if (a.get()->val < i->get()->val) {
                    if(max_size < nodes.size() - 1) {
                        return std::make_pair(i, FULL);
                    }
                    nodes.insert(i, a);
                    return std::make_pair(--i, SUCCESS);
                }
            }
            if(nodes.size() < max_size){
                nodes.push_back(a);
                return std::make_pair(--nodes.end(), SUCCESS);
            } else{
                return std::make_pair(nodes.end(), FULL);
            }
        }

    };

};

template <typename T>
btree<T>::node_list::node_list(const btree<T>::node_list &origin): max_size{origin.max_size} {
    std::cout << "222222 ------------ call coput \n";
    for(const auto& i: origin.nodes){
        auto new_node = std::make_shared<node>(node(i.get()->val, nullptr, origin.max_size));
        if(i.get()->child == nullptr){
            nodes.push_back(new_node);
        }else{
            new_node.get()->child = i.get()->child;
            nodes.push_back(new_node);
        }
    }
}

template <typename T>
btree<T>::node::node(const btree<T>::node &orig) {
    val = orig.val;
    //TODO: ERROR
    std::cout << "1111111 -------call copy construct \n";
    child = orig.child;
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem) {
    auto new_node_pointer = std::make_shared<node>(node{elem, nullptr, max_size});
    auto temp_node_list = root;
    auto inset_flag = temp_node_list.get()->insert(new_node_pointer);
    while( inset_flag.second != SUCCESS ){
        if(inset_flag.second == DUP){
            return std::make_pair(btree_iterator<T>(temp_node_list, inset_flag.first), false);
        }

        if(inset_flag.second == FULL){
            if(inset_flag.first == temp_node_list.get()->nodes.end()){
                //check if last node exist.
                if(temp_node_list.get()->nodes.size() == max_size){
                    // create last node
                    temp_node_list.get()->nodes.push_back(std::make_shared<node>(node(max_size)));
                    inset_flag.first = temp_node_list.get()->nodes.end();
                }
                inset_flag.first--;
                temp_node_list = inset_flag.first->get()->child;
            }else{
                if(inset_flag.first->get()->child == nullptr){
                    inset_flag.first->get()->child = std::make_shared<node_list>(node_list(max_size,btree_iterator<T>{temp_node_list, inset_flag.first}));
                }
                temp_node_list = inset_flag.first->get()->child;
            }
        }
        inset_flag = temp_node_list.get()->insert(new_node_pointer);
    }
    return std::make_pair(btree_iterator<T>(temp_node_list, inset_flag.first), true);
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem) {
    //TODO: more effiect.
    for(auto i = begin(); i != end(); ++i){
        if(i.it->get()->val == elem){
            std::cout << "\n have found: "<< i.it->get()->val << "\n";
            return i;
        }
        if(elem < i.it->get()->val){
            return end();
        }
    }
    return end();
}

template <typename T>
typename btree<T>::iterator btree<T>::end() const {
    if(root.get()->nodes.size() == 0){
        return btree_iterator<T>(root, root.get()->nodes.end());
    }
    auto a = root;
    auto it = a.get()->nodes.end();
    --it;
    while(it->get()->child != nullptr){
        a = it->get()->child;
        it = a.get()->nodes.end();
        --it;
    }
    return btree_iterator<T>(a, a.get()->nodes.end());
}

template <typename T>
btree<T>::btree(const btree<T> &original) {
    max_size = original.max_size;
    root = copy_nodes(*original.root);
}


template <typename T>
typename std::shared_ptr<typename btree<T>::node_list> btree<T>::copy_nodes(const btree<T>::node_list &origin) {
    auto ret = std::make_shared<node_list>(node_list(origin.max_size));
    for(auto i = origin.nodes.begin(); i != origin.nodes.end(); ++i){
        if( i->get()->child == nullptr){
            // if it's last node.
            if( origin.max_size < origin.nodes.size() && i == --origin.nodes.end()){
                ret.get()->nodes.push_back(std::make_shared<node>(node(max_size)));
            } else {
                ret.get()->nodes.push_back(std::make_shared<node>(node(i->get()->val, nullptr, max_size)));
            }
        }else{
            auto new_node = std::make_shared<node>(node(max_size));
            if (!(origin.max_size < origin.nodes.size() && i == -- origin.nodes.end())){
                new_node.get()->val = i->get()->val;
            }
            ret.get()->nodes.push_back(new_node);
            new_node.get()->child = copy_nodes(*i->get()->child.get());
        }
    }
    return ret;
}

template <typename T>
btree<T>::btree(btree<T> &&original): max_size{original.max_size} {
    std::cout<<"call move constructor\n";
    std::swap(this->root, original.root);
}

template <typename T>
btree<T> &btree<T>::operator=(const btree<T> &rhs) {
    if(this == &rhs){
        return *this;
    }
    root.get()->nodes.clear();
    max_size = rhs.max_size;
    root = copy_nodes(*rhs.root);
    return *this;
}

template <typename T>
btree<T> &btree<T>::operator=(btree<T> &&rhs) {
    if(this == &rhs){
        return *this;
    }
    root.get()->nodes.clear();
    max_size = 0;
    std::swap(root, rhs.root);
    std::swap(max_size, rhs.max_size);
    return *this;
}

template <typename T>
typename btree<T>::iterator btree<T>::begin() {
    auto a = root;
    if(a.get()->nodes.size() == 0){
        return btree_iterator<T>(a ,a.get()->nodes.begin());
    }
    while( a.get()->nodes.begin()->get()->child != nullptr){
        a = a.get()->nodes.begin()->get()->child;
    }
    return btree_iterator<T>(a ,a.get()->nodes.begin());
}



template <typename T>
typename btree<T>::iterator btree<T>::end() {
    auto end = root;
    while(true){
        if(end.get()->nodes.size() == 0){
            return btree_iterator<T>(end, end.get()->nodes.end());
        }
        auto it = end.get()->nodes.end();
        --it;
        if(it->get()->child == nullptr){
            return btree_iterator<T>(end, end.get()->nodes.end());
        }
        end = it->get()->child;
    }
}


#endif
