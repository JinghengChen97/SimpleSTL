//
// Created by ch on 2020/11/4.
//

#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>
#include "type_traits.h"

namespace simple_stl {
template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    new (p) T1(value);
}

//for single element, call it's destruction function directly
template<class T>
inline void destroy(T* pointer) {
    pointer->~T();
}

template<class ForwardIterator>
inline void __destroy(ForwardIterator first, ForwardIterator last) {
    typedef typename __type_traits<ForwardIterator>::has_trivial_destructor has_trivial_destructor;
    __destroy_aux(first, last, has_trivial_destructor());
}

//if value type of ForwardIterator has a non-trivial destructor
template <class ForwardIterator>
void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (;first != last; first++) {
        destroy(&*first);
    }
}

//if value type of ForwardIterator has a trivial destructor
template <class ForwardIterator>
void __destroy__aux(ForwardIterator first, ForwardIterator last, __true_type) { }
}



#endif //CONSTRUCT_H
