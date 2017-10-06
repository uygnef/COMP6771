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
template<typename T> class const_btree_iterator;

template<typename T>
class btree_iterator{
public:
    btree_iterator(std::shared_ptr<typename btree<T>::Node> n, size_t i): node{n}, index{i}{}

    T& operator*();

private:
    std::shared_ptr<typename btree<T>::Node> node;
    size_t index;
};



template <typename T>
T &btree_iterator<T>::operator*() {
    return node.get()->elems[index];
}

#endif
