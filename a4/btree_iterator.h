#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include "btree.h"
#include <memory>
/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)
template<typename T> class btree;

template<typename T>
class btree_iterator: std::iterator< std::bidirectional_iterator_tag, T>{
public:
    typedef std::bidirectional_iterator_tag  iterator_category;
    typedef T* pointer;
    typedef T& reference;
    typedef T value_type;

    typedef std::shared_ptr<typename btree<T>::node_set> node_set;
    typedef typename std::set< std::shared_ptr<typename btree<T>::node> >::iterator iter;

    btree_iterator() = default;

    btree_iterator(std::nullptr_t, std::nullptr_t){};
    btree_iterator(node_set pointee,
                   iter index): pointee{pointee}, it{index} {}

    bool operator==(const btree_iterator<T>& other) const;
    btree_iterator& operator++();

public:
    void next();
    void up();
    void down();

    node_set pointee;
    iter it;
};

template<typename T>
class const_btree_iterator {
public:
    typedef std::shared_ptr<typename btree<T>::node_set> node_set;
    typedef typename std::set< std::shared_ptr<typename btree<T>::node> >::const_iterator iter;

    const_btree_iterator(std::nullptr_t, std::nullptr_t){};
    const_btree_iterator(node_set p, iter it): pointee{p}, it{it} {}

private:
    node_set pointee;
    iter it;

};

template<typename T>
bool btree_iterator<T>::operator==(const btree_iterator<T> &other) const {
    return other.pointee == this->pointee && other.it == this->it;
}

template<typename T>
btree_iterator<T> &btree_iterator<T>::operator++() {
    next();
    return *this;
}

template<typename T>
void btree_iterator<T>::next() {
    if(pointee.get()->parent.pointee == nullptr && it == pointee.get()->nodes.end()){

    }
    ++it;
    if(it == pointee.get()->nodes.end()){
        if(pointee.get()->last_child == nullptr){
            it = pointee.get()->parent.it;
            ++it;
            pointee = pointee.get()->parent.pointee;
        }else{
            pointee = pointee.get()->last_child;
            it = pointee.get()->nodes.begin();
        }
    }else if(it->get()->child != nullptr){
        pointee = it->get()->child;
        it = pointee.get()->nodes.begin();
    }

    while(pointee == nullptr || it == pointee.get()->nodes.end()){
        next();
    }
}

template<typename T>
void btree_iterator<T>::up() {
    if(pointee.get()->parent.pointee == nullptr){
        pointee == nullptr;
        it = nullptr;
    } else {
        it = pointee.get()->parent.it;
        pointee = pointee.get()->parent.pointee;
    }
}

#endif
