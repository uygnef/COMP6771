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
bool operator==(const btree_iterator<T, C>& lhs, const btree_iterator   <T, C>& other);

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
    btree_iterator(const btree<Base>* tree , std::shared_ptr<typename btree<Base>::Node> n, size_t i): node{n}, index{i}, tree{tree}{}

    reference operator*() const {
        if(node == nullptr)
            throw("out of range.");
        return node.get()->elems[index];
    };

    pointer operator->() const { return &(operator*());}

//    bool operator==(const btree_iterator<Base, Constness>& rhs);
//    bool operator!=(const btree_iterator<Base, Constness>& rhs);

    btree_iterator& operator++();
    btree_iterator& operator--();
    btree_iterator operator++(int);
    btree_iterator operator--(int);


    const btree<Base>* get_tree() const { return tree;}
    const size_t& get_index() const { return index;}
    const std::shared_ptr<typename btree<Base>::Node>& get_node() const { return node;}

private:
    bool right_down();
    bool up_right();
    bool down_left();
    bool up_left();
    std::shared_ptr<typename btree<value_type >::Node> node;
    size_t index;
    const btree<value_type >* tree;
    friend bool operator==<>(const btree_iterator<Base, Constness>&, const btree_iterator<Base, Constness>&);
};




//template <typename T>
//class const_btree_iterator{
//public:
//    typedef ptrdiff_t                  	difference_type;
//    typedef std::bidirectional_iterator_tag	iterator_category;
//    typedef const T 						   	value_type;
//    typedef const T* 					pointer;
//    typedef const T& 					reference;
//
//    friend class btree_iterator<T>;
//    friend class const_btree_iterator<const T>;
//    const_btree_iterator()= default;
//    const_btree_iterator(const btree<T>* tree, const std::shared_ptr<typename btree<T>::Node>& n1, const size_t& i):
//            tree{tree}, node{n1}, index{i}{}
//
//    reference operator*();
//    inline const T* operator->(){ return  &(operator*());}
//
//    bool operator==(const const_btree_iterator& rhs) ;
//    bool operator!=(const const_btree_iterator& rhs) ;
//
//    const_btree_iterator& operator++();
//    const_btree_iterator& operator--();
//
//
//private:
//    std::shared_ptr<typename btree<T>::Node> node;
//    size_t index;
//    const btree<const T>* tree;
//
//
//};
//
//template <typename T>
//const T&  const_btree_iterator<T>::operator*() {
//    if(node== nullptr){
//        throw("Can not dereference end iterator.");
//    }
//    return node.get()->elems[index];
//}
//
//template <typename T>
//bool const_btree_iterator<T>::operator==(const const_btree_iterator<T> &rhs)  {
//    return node.get() == rhs.node.get() && index == rhs.index;
//}
//
//template <typename T>
//bool const_btree_iterator<T>::operator!=(const const_btree_iterator<T> &rhs)  {
//    return node.get() != rhs.node.get() || index != rhs.index;
//}
//
//template <typename T>
//const_btree_iterator<T> &const_btree_iterator<T>::operator++() {
//    auto temp = btree_iterator<T>(tree, node, index);
//    ++temp;
//    node = temp.node;
//    index = temp.index;
//    return *this;
//}
//
//template <typename T>
//const_btree_iterator<T> &const_btree_iterator<T>::operator--() {
//    auto temp = btree_iterator<T>(tree, node, index);
//    --temp;
//    node = temp.node;
//    index = temp.index;
//    return *this;
//}


//template <typename T, template <typename> class C>
//bool btree_iterator<T, C>::operator==(const btree_iterator<T, C>& rhs) {
//    return node.get() == rhs.node.get() && index == rhs.index;
//}

template <typename T, template <typename> class C>
bool operator==(const btree_iterator<T, C>& lhs, const btree_iterator<T, C>& rhs){
    return lhs.tree == rhs.tree && lhs.node == rhs.node && rhs.index == lhs.index;
}

template <typename T, template <typename> class C>
bool operator!=(const btree_iterator<T, C>& lhs, const btree_iterator<T, C>& rhs){
    return !(rhs == lhs);
}

template <typename T, template <typename> class C>
btree_iterator<T, C> &btree_iterator<T, C>::operator++() {
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

//template <typename T, template <typename> class C>
//bool btree_iterator<T,C>::operator!=(const btree_iterator<T, C>& rhs) {
//    return node != rhs.node || index != rhs.index;
//
//}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::right_down() {
    if(node.get()->children[index+1] == nullptr){
        return false;
    }
    node = node.get()->children[index+1];
    index = 0;
    return true;
}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::up_right() {
    if(node.get()->parent.node == nullptr){
        return false;
    }

    index = node.get()->parent.index;
    node = node.get()->parent.node;

    return node.get()->size() > index;
}

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::down_left() {
    if(node.get()->children[index] == nullptr){
        return false;
    }
    node = node.get()->children[index];
    index = 0;
    return true;
}

template <typename T, template <typename> class C>
btree_iterator<T, C> &btree_iterator<T, C>::operator--() {
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

template <typename T, template <typename> class C>
bool btree_iterator<T, C>::up_left() {
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
