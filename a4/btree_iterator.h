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

    typedef std::shared_ptr<typename btree<T>::node_list> node_list;
    typedef typename std::list< std::shared_ptr<typename btree<T>::node> >::iterator iter;

    btree_iterator() = default;

    btree_iterator(std::nullptr_t, std::nullptr_t){};
    btree_iterator(node_list pointee,
                   iter index): pointee{pointee}, it{index} {}

    bool operator==(const btree_iterator<T>& other) const;
    bool operator!=(const btree_iterator<T>& other) const;

    btree_iterator& operator++();

public:
    bool next();
    bool up();
    bool down();

    inline bool is_end();
    inline bool is_root_list();
    inline bool has_child();
    inline bool is_last_node();

    node_list pointee;
    iter it;
};

template<typename T>
class const_btree_iterator {
public:
    typedef std::shared_ptr<typename btree<T>::node_list> node_list;
    typedef typename std::list< std::shared_ptr<typename btree<T>::node> >::const_iterator iter;

    const_btree_iterator(std::nullptr_t, std::nullptr_t){};
    const_btree_iterator(node_list p, iter it): pointee{p}, it{it} {}

private:
    node_list pointee;
    iter it;

};

template<typename T>
bool btree_iterator<T>::is_end() {
    return it == --pointee.get()->nodes.end() && pointee.get()->max_size < pointee.get()->nodes.size();
}

template<typename T>
bool btree_iterator<T>::operator==(const btree_iterator<T> &other) const {
    return other.pointee == this->pointee && other.it == this->it;
}

template<typename T>
btree_iterator<T> &btree_iterator<T>::operator++() {

    std::cout << " iter: "<< it->get()->val<<std::endl;
    if(is_end()){
        return *this;
    }

    if(down()){
        return *this;
    }

    if(next()){
        return *this;
    }

    up();
    if(is_last_node()){
        pointee = it->get()->child;
        it = pointee.get()->nodes.begin();
    }
    return *this;
}

template<typename T>
bool btree_iterator<T>::next() {
    if(is_end()){
        return false;
    }
    ++it;
    if(is_last_node()){
        down();
    }
    return true;
}

template<typename T>
bool btree_iterator<T>::up() {
    if(is_root_list()){
        it = pointee.get()->nodes.end();
        return false;
    } else {
        it = pointee.get()->parent.it;
        pointee = pointee.get()->parent.pointee;
        while (is_last_node()){
            it = pointee.get()->parent.it;
            pointee = pointee.get()->parent.pointee;
        }
        ++it;
    }
    return true;
}

template<typename T>
bool btree_iterator<T>::down() {
    if(has_child()){
        pointee = it->get()->child;
        it = pointee.get()->nodes.begin();
        return true;
    }     
    return false;
}


template<typename T>
inline bool btree_iterator<T>::is_root_list() {
    return pointee.get()->parent.pointee == nullptr;
}

template<typename T>
bool btree_iterator<T>::has_child() {
    return it->get()->child != nullptr;
}

template<typename T>
bool btree_iterator<T>::operator!=(const btree_iterator<T> &other) const {
    return !(*this == other);
}

template<typename T>
bool btree_iterator<T>::is_last_node() {
    return it == --pointee.get()->nodes.end() &&  pointee.get()->max_size < pointee.get()->nodes.size();
}

#endif
