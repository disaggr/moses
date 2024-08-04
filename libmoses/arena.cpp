#include "arena.h"
#include <iostream>
#include <sys/mman.h>
#include "place.h"

namespace moses
{

    void *Arena::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
                                 size_t alignment, bool *zero, bool *commit, unsigned arena_id)
    {
        // From arena_ind to place
        // place
        // void* new_extent = mmap(new_addr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        void *new_extent = GetPlace()->GetPageManager()->Allocate(size);
        printf("new extent of size %#zx: %p\n", size, new_extent);
        return new_extent;
    }

    bool Arena::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 bool committed, unsigned arena_id)
    {
        GetPlace()->GetPageManager()->Deallocate(addr, size);
        printf("delete extent of size %#zx: %p\n", size, addr);
        return false;
    }

    void Arena::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                  bool committed, unsigned arena_id)
    {
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
    }

    bool Arena::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 size_t offset, size_t length, unsigned arena_id)
    {
        //std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        //same with 'split' we return true
        return true;
    }

    bool Arena::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                   size_t offset, size_t length, unsigned arena_id)
    {
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        return false;
    }

    bool Arena::ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                    size_t offset, size_t length, unsigned arena_id)
    {
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        return false;
    }

    bool Arena::ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                      size_t offset, size_t length, unsigned arena_id)
    {
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        return false;
    }

    bool Arena::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
                                size_t size_b, bool committed, unsigned arena_id)
    {
        //std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        //We implement the 'split' by simply returning true
        return true;
    }

    bool Arena::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
                                size_t size_b, bool committed, unsigned arena_id)
    {
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": Implement me" << std::endl;
        return false;
    }

}
