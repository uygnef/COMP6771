#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>

/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)
template<typename T> class btree_iterator: std::iterator< std::bidirectional_iterator_tag, T>{
public:
    btree_iterator(std::shared_ptr<btree<T>::node> pointee = nullptr): pointee{pointee} {}
private:
    typename std::shared_ptr<btree<T>::node> pointee;
};
#endif
