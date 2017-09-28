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

template<typename T> class btree_iterator: std::iterator< std::bidirectional_iterator_tag, T>{
public:
    typedef std::bidirectional_iterator_tag  iterator_category;
    typedef T* pointer;
    typedef T& reference;
    typedef T value_type;

    typedef std::shared_ptr<typename btree<T>::node_set> node_set;
    typedef typename std::set< std::shared_ptr<typename btree<T>::node> >::const_iterator iter;

    btree_iterator() = delete;


    btree_iterator(node_set pointee,
                   iter index): pointee{pointee}, it{index} {}
private:
    node_set pointee;
    iter it;
};
#endif
