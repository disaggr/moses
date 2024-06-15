#pragma once

#include <string>
#include <jemalloc/jemalloc.h>

namespace moses {

// arenas can be
// fixed: bound to a NUMA node with no means to spill, but be subject to paging
// home: have a fixed home node but may spill to other places as well
// no paging:
// low priority
// 

class BaseArena {
	public:
		BaseArena();
		
		unsigned GetId() {
			return _arena;
		}

        void* ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool *commit, unsigned arena_id);

		bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		void ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		bool ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
				size_t size_b, bool committed, unsigned arena_id);

		bool ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				size_t size_b, bool committed, unsigned arena_id);

	private:
		unsigned _arena;
		extent_hooks_t *_default_hooks;
};

}