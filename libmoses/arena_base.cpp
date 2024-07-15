#include "arena_base.h"
#include "place.h"
#include "extent_hook_dispatch.h"

namespace moses {

BaseArena::BaseArena(Place *place) {
    // Have this arena use the default hooks unless overridden
    size_t sz = sizeof(_default_hooks);
    //Assumption: arena.0 is always present
    je_mallctl("arena.0.extent_hooks", (void *)&_default_hooks, &sz, NULL, 0);

    //Create new arena in jemalloc and couple it with the ExtentHookDispatch hooks
    //TODO: Have the metadata allocated seperately
    unsigned arena_ind;
    size_t unsigned_sz = sizeof(unsigned);
    je_mallctl("arenas.create", (void *) &arena_ind, &unsigned_sz, nullptr, 0);
    _arena = arena_ind;

    //Register this arena object with the jemalloc arena
    ExtentHookDispatch::RegisterArena(this);

    char command[64];
    extent_hooks_t *hooks = ExtentHookDispatch::GetDispatchHooks();
    size_t hooks_sz = sizeof(extent_hooks_t *);
    snprintf(command, sizeof(command), "arena.%u.extent_hooks", arena_ind);
    je_mallctl(command, nullptr, nullptr, (void*) &hooks, sizeof(extent_hooks_t*));
}

void* BaseArena::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
                                 size_t alignment, bool *zero, bool *commit, unsigned arena_id) {
    return _default_hooks->alloc(_default_hooks, new_addr, size, alignment, zero, commit, arena_id);
}

bool BaseArena::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 bool committed, unsigned arena_id) {
    bool ret = _default_hooks->dalloc(_default_hooks, addr, size, committed, arena_id);
    return ret;
}

void BaseArena::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                  bool committed, unsigned arena_id) {
    _default_hooks->destroy(_default_hooks, addr, size, committed, arena_id);
}

bool BaseArena::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                 size_t offset, size_t length, unsigned arena_id) {
    bool ret = _default_hooks->commit(_default_hooks, addr, size, offset, length, arena_id);
    return ret;
}

bool BaseArena::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                   size_t offset, size_t length, unsigned arena_id) {
    bool ret = _default_hooks->decommit(_default_hooks, addr, size, offset, length, arena_id);
    return ret;
}

bool BaseArena::ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                    size_t offset, size_t length, unsigned arena_id) {
    bool ret = _default_hooks->purge_lazy(_default_hooks, addr, size, offset, length, arena_id);
    return ret;
}

bool BaseArena::ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                      size_t offset, size_t length, unsigned arena_id) {
    bool ret = _default_hooks->purge_forced(_default_hooks, addr, size, offset, length, arena_id);
    return ret;
}

bool BaseArena::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
                                size_t size_b, bool committed, unsigned arena_id) {
    bool ret = _default_hooks->split(_default_hooks, addr, size, size_a, size_b, committed, arena_id);
    return ret;
}

bool BaseArena::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
                                size_t size_b, bool committed, unsigned arena_id) {
    bool ret = _default_hooks->merge(_default_hooks, addr_a, size_a, addr_b, size_b, committed, arena_id);
    return ret;
}

Place* BaseArena::GetPlace() {
    // FIXME: this seems to be unused. if we can delete it, that would resolve the cyclic dependency between Place and BaseArane
    //return _pg_stack.top();
    return _place;
}

}
