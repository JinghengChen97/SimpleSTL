#include "alloc.h"
namespace simple_stl {

char *DefaultAlloc::start_free = nullptr;
char *DefaultAlloc::end_free = nullptr;
size_t DefaultAlloc::heap_size = 0;
DefaultAlloc::Obj* volatile DefaultAlloc::free_lists[__NFREELISTS] = {
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr
};

void *DefaultAlloc::Allocate(size_t n) {
    Obj *volatile *current_free_list;
    Obj *result;
    if (n > __MAX_BYTES) return MallocAlloc::allocate(n);
    else {
        //get the list of the adequate blocks
        current_free_list = free_lists + FREELIST_INDEX(n);
        result = *current_free_list;
        if (result == nullptr) { //if there is no available block in free list
            return Refill(ROUND_UP(n));
        } else { //if there is an available block in free list
            *current_free_list = result->free_list_link; // renew the free list
            return result;
        }
    }
}

void DefaultAlloc::Deallocate(void *p, size_t n) {
    Obj * volatile* current_free_list;
    Obj *q = (Obj*)p;
    if (n > __MAX_BYTES) {
        MallocAlloc::deallocate(p, n);
        return;
    } else {
        current_free_list = free_lists + FREELIST_INDEX(n);
        q->free_list_link = *current_free_list;
        *current_free_list = q;
    }
}

void *DefaultAlloc::Reallocate(void *p, size_t old_size, size_t new_size) {
    Deallocate(p, old_size);
    return Allocate(new_size);
}

void *DefaultAlloc::Refill(size_t n) {
    //num of blocks, size of each block is n bytes
    int nobjs = 20;

    //try to get blocks from memory pool
    char* chunk = ChunkAlloc(n, nobjs);

    //if get only one block
    if (nobjs == 1) return chunk;

    //if get more than one block, link these blocks one by one, and update free list
    Obj* volatile *current_free_list = free_lists + FREELIST_INDEX(n);
    Obj *current_block, *next_block;

    *current_free_list = current_block = (Obj*)(chunk + n);
    for (int i = 1; i < nobjs - 1; i++) {
        next_block = (Obj*)((char*)current_block + n);
        current_block->free_list_link = next_block;
        current_block = next_block;
    }
    next_block->free_list_link = nullptr;
    return chunk;
}

char *DefaultAlloc::ChunkAlloc(size_t n, int &nobjs) { // n: bytes quantity of a block; nobjs: num of blocks
    size_t bytes_left = end_free - start_free;
    size_t bytes_required = n * nobjs;
    if (bytes_left >= bytes_required) { //if bytes in memory pool are enough
        char *result = start_free;
        start_free += bytes_required;
        return result;
    } else if (bytes_left >= n) { //if bytes in memory cannot meet the need
        // calculate how many bytes the memory pool can offer
        nobjs = bytes_left / n;
        bytes_required = nobjs * n;

        char *result = start_free;
        start_free += bytes_required;
        return start_free;
    } else { //if bytes in memory pool cannot meet even one block
        size_t bytes_to_get = 2 * bytes_required + ROUND_UP(heap_size >> 4); //@TODO :why heap_size>>4?
        //make full use of the left bytes in memory pool
        if (bytes_left > 0) {
            Obj *volatile *goal_free_list = free_lists + FREELIST_INDEX(bytes_left);
            ((Obj*)start_free)->free_list_link = *goal_free_list;
            *goal_free_list = (Obj*)start_free;
        }
        //get space from heap
        start_free = (char*) malloc(bytes_to_get);
        if (start_free == nullptr) { //fail to get space from heap
            //find if unused and adequate blocks are in free list
            for (int i = n; i < __MAX_BYTES; i += __ALIGN) {
                Obj* volatile* goal_free_list;
                Obj* block;
                goal_free_list = free_lists + FREELIST_INDEX(i);
                block = *goal_free_list;
                if (block != nullptr) {
                    start_free = (char*)block;
                    end_free = start_free + i;
                    return ChunkAlloc(n, nobjs); //recursion to modify "nobjs"
                }
            }
            end_free = nullptr;
        }
    }
    return nullptr;
}
}