#include "extent_hook_dispatch.h"
#include "arena.h"


namespace moses {

static struct CacheAlignedArenaMapping arena_mapping[MALLOCX_ARENA_LIMIT];

extent_hooks_t *ExtentHookDispatch::_dispatch_hooks = (extent_hooks_t *) malloc(sizeof(extent_hooks_t));
static bool extent_hook_dispatch_initialized = ExtentHookDispatch::Initialize();

bool ExtentHookDispatch::Initialize() {
	_dispatch_hooks->alloc = &ExtentHookDispatch::ExtentHookAlloc;
	_dispatch_hooks->dalloc = &ExtentHookDispatch::ExtentHookDAlloc;
	_dispatch_hooks->destroy = &ExtentHookDispatch::ExtentHookDestroy;
	_dispatch_hooks->commit = &ExtentHookDispatch::ExtentHookCommit;
	_dispatch_hooks->decommit = &ExtentHookDispatch::ExtentHookDecommit;
	_dispatch_hooks->purge_lazy = &ExtentHookDispatch::ExtentHookPurgeLazy;
	_dispatch_hooks->purge_forced = &ExtentHookDispatch::ExtentHookPurgeForced;
	_dispatch_hooks->split = &ExtentHookDispatch::ExtentHookSplit;
	_dispatch_hooks->merge = &ExtentHookDispatch::ExtentHookMerge;
    //Initialize jemallocs arena 0 with this?
    return true;
}

extent_hooks_t* ExtentHookDispatch::GetDispatchHooks() {
    return _dispatch_hooks;
}

bool ExtentHookDispatch::RegisterArena(Arena *arena) {
    arena_mapping[arena->GetId()].arena_ptr.store(arena);
    return true;
}

void * ExtentHookDispatch::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
	                                        size_t alignment, bool *zero, bool *commit, unsigned arena_id) {
    Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	void * ret = arena->GetHook()->ExtentHookAlloc(extent_hooks, new_addr, size, alignment, zero, commit, arena_id);
	return ret;
}

bool ExtentHookDispatch::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        bool committed, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookDAlloc(extent_hooks, addr, size, committed, arena_id);
	return success;
}

void ExtentHookDispatch::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        bool committed, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	arena->GetHook()->ExtentHookDestroy(extent_hooks, addr, size, committed, arena_id);
}

bool ExtentHookDispatch::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        size_t offset, size_t length, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookCommit(extent_hooks, addr, size, offset, length, arena_id);
	return success;
}

bool ExtentHookDispatch::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                    	size_t offset, size_t length, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookDecommit(extent_hooks, addr, size, offset, length, arena_id);
	return success;
}

bool ExtentHookDispatch::ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            				size_t offset, size_t length, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookPurgeLazy(extent_hooks, addr, size, offset, length, arena_id);
	return success;
}

bool ExtentHookDispatch::ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            				size_t offset, size_t length, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookPurgeForced(extent_hooks, addr, size, offset, length, arena_id);
	return success;
}

bool ExtentHookDispatch::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
    				                        size_t size_b, bool committed, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookSplit(extent_hooks, addr, size, size_a, size_b, committed, arena_id);
	return success;
}

bool ExtentHookDispatch::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				                            size_t size_b, bool committed, unsigned arena_id) {
	Arena *arena = reinterpret_cast<Arena*>(arena_mapping[arena_id].arena_ptr.load());
	bool success = arena->GetHook()->ExtentHookMerge(extent_hooks, addr_a, size_a, addr_b, size_b, committed, arena_id);
	return success;
}

}