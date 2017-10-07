#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include <memory>
#include "btree.h"

/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)

template<typename T> class btree;
template<typename T> class btree_iterator;
template <typename T>
using const_btree_iterator = btree_iterator<const T>;

template<typename T>
class btree_iterator{

public:
    btree_iterator() = default;
    btree_iterator(const btree<T>* tree , std::shared_ptr<typename btree<T>::Node> n, size_t i): node{n}, index{i}, tree{tree}{}
    T& operator*();
    inline T* operator->() { return &(operator*());}
    bool operator==(btree_iterator rhs);
    btree_iterator& operator++();
    btree_iterator& operator--();

    bool operator!=(btree_iterator rhs);

private:
    bool right_down();
    bool up_right();
    bool down_left();
    bool up_left();
    std::shared_ptr<typename btree<T>::Node> node;
    size_t index;
    const btree<T>* tree;
};





template <typename T>
T &btree_iterator<T>::operator*() {
    if(node == nullptr){
        throw("Can not dereference end iterator.");
    }
    return node.get()->elems[index];
}

template <typename T>
bool btree_iterator<T>::operator==(btree_iterator<T> rhs) {
    return node.get() == rhs.node.get() && index == rhs.index;
}

template <typename T>
btree_iterator<T> &btree_iterator<T>::operator++() {
    if(node.get()->parent.node == nullptr && index > node.get()->size() ){
        return *this;
    }

    bool res = right_down();
    // go to left-down most child
    if(res){
        while(down_left());
        return *this;
    }
    // check if it's last node
    if(index != node.get()->size() - 1){
        ++index;
        return *this;
    }

    res = up_right();
    while(!res){
        res = up_right();
        if (node.get()->parent.node == nullptr){
            node = nullptr;
            index = 0;
            return *this;
        }
    }
    return *this;
}

template <typename T>
bool btree_iterator<T>::operator!=(btree_iterator rhs) {
    return node != rhs.node || index != rhs.index;

}

template <typename T>
bool btree_iterator<T>::right_down() {
    if(node.get()->children[index+1] == nullptr){
        return false;
    }
    node = node.get()->children[index+1];
    index = 0;
    return true;
}

template <typename T>
bool btree_iterator<T>::up_right() {
    if(node.get()->parent.node == nullptr){
        return false;
    }

    index = node.get()->parent.index;
    node = node.get()->parent.node;

    return node.get()->size() > index;
}

template <typename T>
bool btree_iterator<T>::down_left() {
    if(node.get()->children[index] == nullptr){
        return false;
    }
    node = node.get()->children[index];
    index = 0;
    return true;
}

template <typename T>
btree_iterator<T> &btree_iterator<T>::operator--() {
    if(node == nullptr){
        const auto& i = tree->pre_end();
        node = i.node;
        index = i.index;
        return *this;
    }

    bool res = down_left();
    if(res){
        index = node.get()->size() - 1;
        while(res){
            res = right_down();
            if(res){
                index = node.get()->size() - 1 ;
            }
        }
        return *this;
    }
    if(index != 0){
        --index;
        return *this;
    }

    while(!up_left()){
        if( node.get()->parent.node == nullptr)
            break;
    }
    return *this;
}

template <typename T>
bool btree_iterator<T>::up_left() {
    // have got root
    if(node.get()->parent.node == nullptr){
        return false;
    }

    index = node.get()->parent.index;
    node = node.get()->parent.node;
    if(index != 0){
        --index;
        return true;
    }
    return false;
}


#endif
