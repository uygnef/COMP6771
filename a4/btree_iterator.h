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
    bool next();

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

//template<typename T>
//btree_iterator<T> &btree_iterator::operator++() {
//    ++it;
//    auto next = *it;
//    if(it == pointee.get()->nodes.end()){
//        if(pointee.get()->last_child != nullptr){
//            next = pointee.get()->last_child;
//        }else{
//
//        }
//    }
//
//    if(next.get()->child != nullptr){
//        pointee = next.get()->child;
//        it = next.get()->child.get()->nodes.begin();
//    }else{
//
//    }
//
//
//    return <#initializer#>;
//}
//
//template<typename T>
//bool btree_iterator<T>::next() {
//    ++it;
//    if(it == pointee.get()->nodes.end()){
//        if(pointee.get()->last_child == nullptr){
//            pointee = pointee.get()->parent;
//            it = pointee
//        }
//    }
//    return false;
//}

#endif
