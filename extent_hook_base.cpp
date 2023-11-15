#include "extent_hook_base.h"

namespace moses {

ExtentHookBase::ExtentHookBase() {
	size_t sz = sizeof(extent_hooks_t);
	//Assumption: arena.0 is always present
	je_mallctl("arena.0.extent_hooks", (void *)&_default_hooks, &sz, NULL, 0);
}

void* ExtentHookBase::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				                        size_t alignment, bool *zero, bool *commit, unsigned arena_id) {
	return _default_hooks->alloc(_default_hooks, new_addr, size, alignment, zero, commit, arena_id);
}

bool ExtentHookBase::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				                        bool committed, unsigned arena_id) {
	bool ret = _default_hooks->dalloc(_default_hooks, addr, size, committed, arena_id);
	return ret;
}

void ExtentHookBase::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				                        bool committed, unsigned arena_id) {
	_default_hooks->destroy(_default_hooks, addr, size, committed, arena_id);
}

bool ExtentHookBase::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
		                        		size_t offset, size_t length, unsigned arena_id) {
	bool ret = _default_hooks->commit(_default_hooks, addr, size, offset, length, arena_id);
	return ret;
}

bool ExtentHookBase::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            			size_t offset, size_t length, unsigned arena_id) {
	bool ret = _default_hooks->decommit(_default_hooks, addr, size, offset, length, arena_id);
	return ret;
}

bool ExtentHookBase::ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            				size_t offset, size_t length, unsigned arena_id) {
	bool ret = _default_hooks->purge_lazy(_default_hooks, addr, size, offset, length, arena_id);
	return ret;
}

bool ExtentHookBase::ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                        	size_t offset, size_t length, unsigned arena_id) {
	bool ret = _default_hooks->purge_forced(_default_hooks, addr, size, offset, length, arena_id);
	return ret;
}

bool ExtentHookBase::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
                                	size_t size_b, bool committed, unsigned arena_id) {
	bool ret = _default_hooks->split(_default_hooks, addr, size, size_a, size_b, committed, arena_id);
	return ret;
}

bool ExtentHookBase::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
                				size_t size_b, bool committed, unsigned arena_id) {
	bool ret = _default_hooks->merge(_default_hooks, addr_a, size_a, addr_b, size_b, committed, arena_id);
	return ret;
}
}