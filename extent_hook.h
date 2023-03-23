#pragma once

namespace moses {

// This uses jemalloc internals MALLOCX_ARENA_LIMIT to verify that the array is big enough to hold
// all possible arena indices
std::atomic<uintptr_t> arena_mapping __attribute__ ((aligned (CACHELINE_SIZE))) = [MALLOCX_ARENA_LIMIT];

class ExtentHookBase {
	public:
		ExtendHookBase() {
			size_t sz = sizeof(extent_hooks_t);
			//Assumption: arena.0 is always present
			mallctl("arena.0.extent_hooks", (void *)&_default_hooks, &sz, NULL, 0)
		}

		bool ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_id) {
			_default_hooks->alloc(default_hooks, new_addr, size, alignment, zero, commit, arena_id);
		}

		bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id) {
			_default_hooks->dalloc(default_hooks, addr, size, committed, arena_id);
		}

		bool ExtentHookDestroy(extent_hook_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id) {
			_default_hooks->destroy(default_hooks, addr, size, committed, arena_id);
		}

		bool ExtentHookCommit(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			_default_hooks->commit(_default_hooks, addr, size, offset, length, arena_id);
		}

		bool ExtentHookDecommit(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			_default_hooks->decommit(_default_hooks, addr, size, offset, length, arena_id);
		}

		bool ExtentHookPurge(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			_default_hooks->purge(_default_hooks, addr, size, offset, length, arena_id);
		}

		bool ExtentSplit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t size_a,
				size_t size_b, bool committed, unsigned arena_id) {
			_default_hooks->split(_default_hooks, addr, size_a, size_b, committed, arena_id);
		}

		bool ExtentMerge(extent_hook_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				size_t size_b, bool committed, unsigned arena_id) {
			_default_hooks->merge(_default_hooks, addr_a, size_a, addr_b, size_b, committed, arena_id);
		}

	private:
		extent_hooks_t *_default_hooks;
}

class ExtendHookNuma : ExtentHookBase {
	public:
		ExtendHookNuma(unsigned node) : ExtentHookBase(), _node(node) {
		}
        
		void *ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_ind) {
			new_addr = numa_alloc_onnode(size, _node);
			return new_addr;
		}
	private:
		unsigned _node;
}

class ExtendHookFile : ExtentHookBase {
	public:
		ExtendHookFile(std::string file_backing) : ExtentHookBase(), _file_backing(file_backing) {
		}
	private:
		std::string _file_backing;
}

class ExtendHookSpilling {
}

class ExtentHookDispatch {
//TODO: Implement the required jemalloc hooks and dispatch them according to the arena_id to the respective objects
	public:
		ExtentHookDispatch() {
			_dispatch_hooks->alloc = &ExtentHookAlloc;
			_dispatch_hooks->dalloc = &ExtentHookDAlloc;
			_dispatch_hooks->destroy = &ExtentHookDestroy;
			_dispatch_hooks->commit = &ExtentHookCommit;
			_dispatch_hooks->decommit = &ExtentHookDecommit;
			_dispatch_hooks->purge_lazy = &ExtentHookPurgeLazy;
			_dispatch_hooks->purge_forced = &ExtentHookPurgeForced;
			_dispatch_hooks->split = &ExtentHookSplit;
			_dispatch_hooks->merge
		}

		extent_hooks_t* GetDispatchHooks() {
            return _dispatch_hooks;
		}

		static bool ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookAlloc(extent_hooks, new_addr, size, alignment, zero, commit, arena_id);
		}

		static bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookDAlloc(default_hooks, addr, size, committed, arena_id);
		}

		static bool ExtentHookDestroy(extent_hook_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookDestroy(default_hooks, addr, size, committed, arena_id);
		}

		static bool ExtentHookCommit(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookCommit(_default_hooks, addr, size, offset, length, arena_id);
		}

		static bool ExtentHookDecommit(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookDecommit(_default_hooks, addr, size, offset, length, arena_id);
		}

		static bool ExtentHookPurge(extent_hook_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookPurge(_default_hooks, addr, size, offset, length, arena_id);
		}

		static bool ExtentHookSplit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t size_a,
				size_t size_b, bool committed, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookSplit(_default_hooks, addr, size_a, size_b, committed, arena_id);
		}

		static bool ExtentHookMerge(extent_hook_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				size_t size_b, bool committed, unsigned arena_id) {
			Arena *arena = reinterpret_cast<Arena*> arena_mapping[arena_id];
			arena->GetHook->ExtentHookMerge(_default_hooks, addr_a, size_a, addr_b, size_b, committed, arena_id);
		}

		private:
			extent_hooks_t *_dispatch_hooks;
}

}