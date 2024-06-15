#pragma once

#include <atomic>
#include "common.h"
#include "arena.h"

namespace moses {

// This uses jemalloc internals MALLOCX_ARENA_LIMIT to verify that the array is big enough to hold
// all possible arena indices
// TODO: Check if cacheline aligned
struct alignas(CACHELINE_SIZE) CacheAlignedArenaMapping {
	union {
		std::atomic<BaseArena *> arena_ptr;
		char padding[CACHELINE_SIZE];
	};
};

class ExtentHookDispatch {
	public:
		static bool Initialize();

		static extent_hooks_t* GetDispatchHooks();

        static bool RegisterArena(BaseArena *arena);

		static void * ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool *commit, unsigned arena_id);

		static bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		static void ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		static bool ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		static bool ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		static bool ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

        static bool ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		static bool ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
				size_t size_b, bool committed, unsigned arena_id);

		static bool ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				size_t size_b, bool committed, unsigned arena_id);

		private:
			ExtentHookDispatch();

			static extent_hooks_t *_dispatch_hooks;
};
}