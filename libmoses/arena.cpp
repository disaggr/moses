#include "arena.h"
#include <iostream>
#include <sys/mman.h>
#include "place.h"
#include <string.h>
#include "log.h"

namespace moses
{

    void *Arena::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
                                 size_t alignment, bool *zero, bool *commit, unsigned arena_id)
    {
        LOG("arena.%d.extent.alloc: new_addr: %p, size: %#zx, alignment: %#zx, zero: %i, commit: %i", arena_id, new_addr, size, alignment, *zero, *commit);

        void *new_extent = NULL;
        if (new_addr != NULL) {
            LOG("arena.%d.extent.alloc: not supported.", arena_id);
            return NULL;
        }

        // From arena_ind to place
        new_extent = GetPlace()->GetPageManager()->Allocate(size, alignment);
        if (!new_extent) {
            LOG("arena.%d.extent.alloc: allocate failed.", arena_id);
            return NULL;
        }
        if ((uintptr_t)new_extent & (alignment - 1)) {
            LOG("arena.%d.extent.alloc: alignment error: %p", arena_id, new_extent);
            GetPlace()->GetPageManager()->Deallocate(new_extent, size);
            return NULL;
        }

        *commit = 1;
        if (*zero) {
            memset(new_extent, 0, size);
        }

        LOG("arena.%d.extent.alloc: result: %p", arena_id, new_extent);

        return new_extent;
    }

    bool Arena::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 bool committed, unsigned arena_id)
    {
        LOG("arena.%d.extent.dalloc: addr: %p, size: %#zx, commit: %i", arena_id, addr, size, committed);

        // opt out.
        return true;
    }

    void Arena::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                  bool committed, unsigned arena_id)
    {
        LOG("arena.%d.extent.destroy: addr: %p, size: %#zx, committed: %i", arena_id, addr, size, committed);

        GetPlace()->GetPageManager()->Deallocate(addr, size);
    }

    bool Arena::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 size_t offset, size_t length, unsigned arena_id)
    {
        LOG("arena.%d.extent.commit: addr: %p, size: %#zx, offset: %#zx, length: %#zx", arena_id, addr, size, offset, length);

        // report success.
        return false;
    }

    bool Arena::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                   size_t offset, size_t length, unsigned arena_id)
    {
        LOG("arena.%d.extent.decommit: addr: %p, size: %#zx, offset: %#zx, length: %#zx", arena_id, addr, size, offset, length);

        // report failure (opt out).
        return true;
    }

    bool Arena::ExtentHookPurge(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                    size_t offset, size_t length, unsigned arena_id)
    {
        LOG("arena.%d.extent.purge: addr: %p, size: %#zx, offset: %#zx, length: %#zx", arena_id, addr, size, offset, length);

        // report failure (opt out).
        int res = madvise(addr, size, MADV_REMOVE);
        if (res != 0) {
            throw std::runtime_error("Failed to dellocate memory");
        }
        return true;
    }

    bool Arena::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
                                size_t size_b, bool committed, unsigned arena_id)
    {
        LOG("arena.%d.extent.split: addr: %p, size: %#zx (%#zx -- %#zx), committed: %i", arena_id, addr, size, size_a, size_b, committed);

        // report success.
        return false;
    }

    bool Arena::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
                                size_t size_b, bool committed, unsigned arena_id)
    {
        LOG("arena.%d.extent.merge: addr_a: %p, size_a: %#zx, addr_b: %p, size_b: %#zx, committed: %i", arena_id, addr_a, size_a, addr_b, size_b, committed);

        // report success.
        return false;
    }

}
