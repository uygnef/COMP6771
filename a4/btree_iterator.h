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

template <typename T>
struct Identity{
    using type = T;
};
template<typename T, template <typename U> class Constness> class btree_iterator;

template <typename T, template <typename> class C>
bool operator==(const btree_iterator<T, C>& lhs, const btree_iterator<T, C>& rhs);

template<typename Base, template <typename U> class Constness = Identity>
class btree_iterator{
    using T = typename Constness<Base>::type;
public:

    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag	iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

//    friend btree<Base>;

    btree_iterator() = default;
    btree_iterator(const btree<Base>* tree_ , std::shared_ptr<typename btree<Base>::Node> n, size_t i): node{n}, index{i}, tree{tree_}{}

    reference operator*() const {
        return node.lock().get()->elems[index];
    };

    pointer operator->() const { return &(operator*());}

    btree_iterator& operator++();
    btree_iterator& operator--();
    btree_iterator operator++(int);
    btree_iterator operator--(int);


    const btree<Base>* get_tree() const { return tree;}
    const size_t& get_index() const { return index;}
    const std::shared_ptr<typename btree<Base>::Node>& get_node() const { return node.lock();}

    std::weak_ptr<typename btree<Base>::Node> node;
    size_t index{};
    const btree<Base>* tree;

private:
    bool right_down();
    bool up_right();
    bool down_left();
    bool up_left();

    friend bool operator==<>(const btree_iterator<Base, Constness>&, const btree_iterator<Base, Constness>&);
};

template <typename T, template <typename> class C>
bool operator==(const btree_iterator<T, C>& lhs, const btree_iterator<T, C>& rhs){
    return lhs.tree == rhs.tree && lhs.node.lock() == rhs.node.lock() && rhs.index == lhs.index;
}

template <typename T, template <typename> class C>
bool operator!=(const btree_iterator<T, C>& lhs, const btree_iterator<T, C>& rhs){
    return !(rhs == lhs);
}

template <typename T, template <typename> class C>
btree_iterator<T, C> &btree_iterator<T, C>::operator++() {

    if(node.lock().get()->parent.node.lock() == nullptr && index >= node.lock().get()->size() ){
        return *this;
    }

    bool res = right_down();
    // go to left-down most child
    if(res){
        while(down_left());
        return *this;
    }
    // check if it's last node
    if(index != node.lock().get()->size() - 1){
        ++index;
        return *this;
    }

    res = up_right();
    while(!res){
        res = up_right();
        if (node.lock().get()->parent.node.lock() == nullptr){
            node = std::weak_ptr<typename btree<T>::Node>();
            index = 0;
            return *this;
        }
    }
    return *this;
}

//template <typename T, template <typename> class C>
//bool btree_iterator<T,C>::operator!=(const btree_iterator<T, C>& rhs) {
//    return node != rhs.node || index != rhs.index;
//
//}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::right_down() {
    if(node.lock().get()->children[index+1] == nullptr){
        return false;
    }
    node = node.lock().get()->children[index+1];
    index = 0;
    return true;
}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::up_right() {
    if(node.lock().get()->parent.node.lock() == nullptr){
        return false;
    }

    index = node.lock().get()->parent.index;
    node = node.lock().get()->parent.node;

    return node.lock().get()->size() > index;
}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::down_left() {
    if(node.lock().get()->children[index] == nullptr){
        return false;
    }
    node = node.lock().get()->children[index];
    index = 0;
    return true;
}

template <typename T, template <typename> class C>
btree_iterator<T, C> &btree_iterator<T, C>::operator--() {
    if(node.lock() == nullptr){
        const auto& i = tree->pre_end();
        node = i.node;
        index = i.index;
        return *this;
    }

    bool res = down_left();
    if(res){
        index = node.lock().get()->size() - 1;
        while(res){
            res = right_down();
            if(res){
                index = node.lock().get()->size() - 1 ;
            }
        }
        return *this;
    }
    if(index != 0){
        --index;
        return *this;
    }

    while(!up_left()){
        if( node.lock().get()->parent.node.lock() == nullptr)
            break;
    }
    return *this;
}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::up_left() {
    // have got root
    if(node.lock().get()->parent.node.lock() == nullptr){
        return false;
    }

    index = node.lock().get()->parent.index;
    node = node.lock().get()->parent.node;
    if(index != 0){
        --index;
        return true;
    }
    return false;
}

template <typename T, template <typename> class C>
btree_iterator<T, C> btree_iterator<T, C>::operator++(int) {
    btree_iterator ret = *this;
    operator++();
    return ret;
}

template <typename T, template <typename> class C>
btree_iterator<T, C> btree_iterator<T, C>::operator--(int) {
    btree_iterator ret = *this;
    operator--();
    return ret;
}


#endif
