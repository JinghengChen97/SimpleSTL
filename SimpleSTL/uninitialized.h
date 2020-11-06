//
// Created by ch on 2020/11/6.
//

#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H
#include "type_traits.h"
#include "iterator_traits.h"
#include "construct.h"

namespace simple_stl {
/// uninitialized_fill_n
template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type) {
    for (; n > 0; n--, first++) {
        *first = x;
    }
    return first;
}

template <class ForwardIterator, class Size, class T>
ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type) {
    for (; n > 0; n--, first++) {
        construct(&*first, x);
    }
    return first;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type POD;
    return __uninitialized_fill_n_aux(first, n, x, POD());
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
    return __uninitialized_fill_n(first, n, x, value_type<ForwardIterator>(first));
}

/// uninitialized_copy
template <class InputIterator, class ForwardIterator>
ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
    for ( ;first != last; first++, result++) {
        *first = *result;
    }
    return result;
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
    for ( ;first != last; first++, result++) {
        construct(&*first, *result);
    }
    return result;
}

template <class InputIterator, class ForwardIterator, class T1>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T1*) {
    typedef typename __type_traits<T1>::is_POD_type POD;
    return __uninitialized_copy_aux(first, last, result, POD());
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    return __uninitialized_copy(first, last, result, value_type(result));
}

/// uninitialized_fill
template <class ForwardIterator, class T>
ForwardIterator
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
    for ( ; first != last; first++) {
        *first = x;
    }
    return first;
}

template <class ForwardIterator, class T>
ForwardIterator
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
    for ( ; first != last; first++) {
        construct(&*first, x);
    }
    return first;
}

template <class ForwardIterator, class T, class T1>
inline ForwardIterator
__uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type POD;
    return __uninitialized_fill_aux(first, last, x, POD());
}

template <class ForwardIterator, class T>
inline ForwardIterator
uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
    return __uninitialized_fill(first, last, x, value_type(first));
}
}
#endif //UNINITIALIZED_H
