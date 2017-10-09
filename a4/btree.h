/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node 
 * that stores m client elements partition the tree 
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>
#include <memory>
//#include <bits/shared_ptr.h>
#include <exception>
#include <queue>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

template <typename T> 
class btree {
 public:
  /** Hmm, need some iterator typedefs here... friends? **/

    using iterator = btree_iterator<T> ;
    friend iterator;

    using const_iterator = btree_iterator<T, std::add_const> ;
    friend const_iterator ;

    using reverse_iterator =  std::reverse_iterator<iterator> ;
    friend reverse_iterator;

    using const_reverse_iterator = std::reverse_iterator<const_iterator> ;
    friend const_reverse_iterator;
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
  btree(size_t maxNodeElems = 40): max_size{maxNodeElems}{
      root = std::make_shared<Node>(Node(maxNodeElems));
  }

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
  btree(btree<T>&& original) noexcept;
  
  
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
  friend std::ostream& operator<< (std::ostream& os, const btree<T>& tree){
      std::queue<std::shared_ptr<Node>> queue;
      queue.push(tree.root);
      while(!queue.empty()){
          tree.print_tree(os, queue);
      }
      return os;
  };



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

    iterator begin() const;
    inline iterator end() const{ return btree_iterator<T>(this, nullptr, 0);}
    reverse_iterator 		rbegin() 	const;
    reverse_iterator 		rend() 		const;
    const_iterator 			cbegin() 	const;
    const_iterator 			cend() 		const;
    const_reverse_iterator 	crbegin() 	const;
    const_reverse_iterator 	crend() 	const;
  
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
    const_iterator find(const T& elem) const;

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
  ~btree(){
      max_size = 0;
      root.get()->elems.clear();
      root.get()->children.clear();
  };


// The details of your implementation go here
struct Node{
    friend btree;


    Node(size_t max_size): elems{}, max_size{max_size}, children(1) {}  //chilren size = elems size + 1
    Node(T val, size_t max_size): elems{val}, max_size{max_size}, children(2){}
    Node(T val, iterator parent, size_t max_size):
            elems{val}, parent{parent}, max_size{max_size}, children(2){}

    ~Node(){
        elems.clear();
        children.clear();
    }

    inline bool is_full(){ return elems.size() == max_size;}
    inline size_t size(){ return elems.size();}

    //copy value if vector is full, push back.
    inline void copy_elem_insert(const size_t& i, T val){
        if(i < size()){
            elems[i] = val;
        }else{
            elems.push_back(val);
        }
    }
    inline void copy_child_insert(const size_t& i, const std::shared_ptr<Node>& ch){
        if(i < children.size()){
            children[i] = ch;
        }else{
            children.push_back(ch);
        }
    }
    std::pair<bool, size_t> find(T) const;

    std::vector<T> elems;
    iterator parent;
    size_t max_size;
    std::vector<typename std::shared_ptr<Node>> children;
};

private:

    void print_tree(std::ostream& os, std::queue<typename std::shared_ptr<Node>>& node_queue) const {

        auto now_pointer = node_queue.front();
        node_queue.pop();
        for(const auto& i: now_pointer.get()->elems){
            os << i << " ";
        }
        for(const auto& i: now_pointer.get()->children){
            if(i != nullptr){
                node_queue.push(i);
            }
        }
    }

    void copy_helper(const std::shared_ptr<Node>& old, std::shared_ptr<Node>& new_list);
    iterator pre_end() const;

    size_t max_size;
    std::shared_ptr<Node> root;
};

template <typename T>
std::pair<bool, size_t> btree<T>::Node::find(T key) const{
    size_t i;
    for(i = 0 ; i < elems.size(); ++i){
        if(elems[i] == key){
            return std::make_pair(true, i);
        }
        if(key < elems[i]){
            return std::make_pair(false, i);
        }
    }
 //   std::cout << "find key: "<< key << " position: " << i << std::endl;
    return std::make_pair(false, i);
}



template <typename T>
std::pair<btree_iterator<T>, bool> btree<T>::insert(const T &elem) {
    auto temp_node = root;
    while(true){
        if(temp_node.get()->is_full()){
            auto result_pair = temp_node.get()->find(elem);
            if(result_pair.first) {
                return std::make_pair(btree_iterator<T>(this, temp_node, result_pair.second), false);
            }
            if(temp_node.get()->children[result_pair.second] == nullptr){
                auto child_node = std::make_shared<Node>(Node(elem, max_size));
                temp_node.get()->children[result_pair.second] = child_node;
                child_node.get()->parent = btree_iterator<T>(this, temp_node, result_pair.second);
                btree_iterator<T> iter(this, temp_node->children[result_pair.second], 0);
                return std::make_pair(iter, true);
            }
            temp_node = temp_node->children[result_pair.second];
        }else{
            auto result_pair = temp_node.get()->find(elem);
            if(result_pair.first){
                return std::make_pair(btree_iterator<T>(this, temp_node, result_pair.second), false);
            }
            const auto& temp_iter = temp_node.get()->elems.begin() + result_pair.second;
            temp_node.get()->elems.insert(temp_iter, elem);
            /*
             * Node can not be remove, so if parent node is not full, all children of this not are nullptr.
             *   we do not need to add new elem's child into right place.
             */
            temp_node.get()->children.push_back(nullptr);
            return std::make_pair(btree_iterator<T>(this ,temp_node, result_pair.second), true);
        }
    }
}

template <typename T>
btree<T>::btree(const btree<T> &original) {
    max_size = original.max_size;
    root = std::make_shared<Node>(Node(max_size));
    copy_helper(original.root, root);
}

template <typename T>
void btree<T>::copy_helper(const std::shared_ptr<Node>& old, std::shared_ptr<Node>& new_list) {
    size_t i;
    for(i = 0; i < old.get()->elems.size(); ++i){
        new_list.get()->copy_elem_insert(i, old.get()->elems[i]);
    }

    for(i=0; i < old.get()->children.size(); ++i){
        if(old.get()->children[i] == nullptr){
            new_list.get()->copy_child_insert(i, nullptr);
        } else {
            auto new_child_node = std::make_shared<Node>(Node(old.get()->max_size));
            new_child_node.get()->parent = btree_iterator<T>(this, new_list, i);
            new_list.get()->copy_child_insert(i, new_child_node);
            copy_helper(old.get()->children[i], new_child_node);
        }
    }
}


template <typename T>
btree<T>::btree(btree<T> &&original) noexcept {
    root = std::make_shared<Node>(Node(0));
    max_size = 0;
    std::swap(original.root, root);
    std::swap(original.max_size, max_size);
}

template <typename T>
btree<T> &btree<T>::operator=(const btree<T> &rhs) {
    if(this == &rhs){
        return *this;
    }
    root.get()->elems.clear();
    root.get()->children.clear();
    max_size = rhs.max_size;
    copy_helper(rhs.root, root);
    return *this;
}

template <typename T>
btree<T> &btree<T>::operator=(btree<T> &&rhs) {
    if(this == &rhs){
        return *this;
    }
    root.get()->elems.clear();
    root.get()->children.clear();
    max_size = 0;

    std::swap(root, rhs.root);
    std::swap(max_size, rhs.max_size);

    return *this;
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem)  {
    auto temp_node = root;
    size_t i;
    while(temp_node != nullptr){
        for(i = 0; i < temp_node.get()->elems.size(); ++i){
            if(temp_node.get()->elems[i] == elem){
                return btree<T>::iterator(this, temp_node, i);
            }
            if(elem < temp_node.get()->elems[i]){
                break;
            }
        }
        temp_node = temp_node.get()->children[i];
    }

    return end();
}



template <typename T>
typename btree<T>::iterator btree<T>::begin() const {
    if(root.get()->size() == 0){
        return btree_iterator<T>(this, nullptr, 0);
    }
    auto temp_node = root;
    auto pre_temp = root;
    while(temp_node != nullptr){
        pre_temp = temp_node;
        temp_node = temp_node.get()->children[0];
    }

    return btree_iterator<T>(this, pre_temp, 0);
}

template <typename T>
typename btree<T>::iterator btree<T>::pre_end() const {
    auto temp_node = root;
    auto last_child_val = temp_node.get()->children[temp_node.get()->children.size()-1];
    while(last_child_val != nullptr){
        temp_node = last_child_val;
        last_child_val = temp_node.get()->children[temp_node.get()->children.size()-1];
    }
    return btree_iterator<T>(this, temp_node, temp_node.get()->elems.size()-1 );
}

template <typename T>
typename btree<T>::reverse_iterator btree<T>::rbegin() const {
    return btree<T>::reverse_iterator(end());
}

template <typename T>
typename btree<T>::reverse_iterator btree<T>::rend() const {
    return btree<T>::reverse_iterator(begin());
}

template <typename T>
typename btree<T>::const_iterator btree<T>::cbegin() const {
    return btree::const_iterator(begin().get_tree(), begin().get_node(), begin().get_index());
}

template <typename T>
typename btree<T>::const_iterator btree<T>::cend() const {
    auto it = end();
    return btree<T>::const_iterator(it.get_tree(), it.get_node(), it.get_index());
}

template <typename T>
typename btree<T>::const_reverse_iterator btree<T>::crbegin() const {
    return btree<T>::const_reverse_iterator(cend());
}

template <typename T>
typename btree<T>::const_reverse_iterator btree<T>::crend() const {
    return btree<T>::const_reverse_iterator(cbegin());
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T &elem) const {
    auto temp_node = root;
    size_t i;
    while(temp_node != nullptr){
        for(i = 0; i < temp_node.get()->elems.size(); ++i){
            if(temp_node.get()->elems[i] == elem){
                return btree<T>::const_iterator(this, temp_node, i);
            }
            if(elem < temp_node.get()->elems[i]){
                break;
            }
        }
        temp_node = temp_node.get()->children[i];
    }

    return cend();
}


#endif
