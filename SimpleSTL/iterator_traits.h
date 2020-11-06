//
// Created by ch on 2020/11/6.
//

#ifndef ITERATOR_TRAITS_H
#define ITERATOR_TRAITS_H
#include <cstddef>

namespace simple_stl {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class Iterator>
struct __iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};

//for built-in non-to-const pointers
//T is it's origin type
template <class T>
struct __iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};

//for built-in to-const pointers
//T is it's origin type
template <class T>
struct __iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

template <class Iterator>
typename __iterator_traits<Iterator>::value_type *
value_type() {
    return static_cast<typename __iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
typename __iterator_traits<Iterator>::difference_type *
distance_type() {
    return static_cast<typename __iterator_traits<Iterator>::difference_type*>(0);
}

template <class Iterator>
typename __iterator_traits<Iterator>::iterator_category
iterator_category() {
    typedef typename __iterator_traits<Iterator>::iterator_category category;
    return category();
}

}

#endif //ITERATOR_TRAITS_H

