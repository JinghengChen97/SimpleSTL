//
// Created by ch on 2020/11/3.
//
#ifndef SIMPLESTL_ALLOC_H
#define SIMPLESTL_ALLOC_H
#include <stdlib.h> // for malloc() and free()
#include <stddef.h> // for size_t
namespace simple_stl {
/*
 * top level space allocator
 * unlike source code in STL, code here doesn't handle the situation of out of memory.
 * */
class MallocAlloc {
public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t /* n */) {
        free(p);
    }

    static void *reallocate(void *p, size_t  old_size, size_t new_size) {
        void *result = reallocate(p, old_size, new_size);
        return result;
    }
};

enum {__ALIGN = 8}; //for Round-Up of small blocks
enum {__MAX_BYTES = 128}; //max size of a block
enum {__NFREELISTS = 16}; //num of free lists
/*
 * sub-level space allocator
 * */
class DefaultAlloc {
private:
    union Obj {
        union Obj *free_list_link;
        char client_data[1];
    };
private:
    static size_t ROUND_UP(size_t bytes) {
        return ((bytes + __ALIGN - 1) & (~(__ALIGN - 1)));
    }
    static size_t FREELIST_INDEX(size_t bytes) {
        return (bytes + __ALIGN - 1) / __ALIGN - 1;
    }
private:
    static void *Refill(size_t n);
    static char *ChunkAlloc(size_t n, int& nobjs);
private:
    static Obj* volatile free_lists[__NFREELISTS]; //volatile : protection for multi-threaded situation
    static char *start_free;
    static char *end_free;
    static size_t heap_size;
public:
    static void *Allocate(size_t n);
    static void Deallocate(void *p, size_t n);
    static void *Reallocate(void *p, size_t old_size, size_t new_size);
};


}

#endif //SIMPLESTL_ALLOC_H
