#pragma once

#include <string>
#include "common.h"
#include "extent_hook_base.h"

namespace moses {

#ifdef __NUMA__
class ExtentHookNuma : ExtentHookBase {
	public:
		ExtentHookNuma(unsigned node) : ExtentHookBase(), _node(node) {
		}
        
		void *ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool commit, unsigned arena_ind) {
			new_addr = numa_alloc_onnode(size, _node);
			return new_addr;
		}
	private:
		unsigned _node;
};
#endif

class ExtentHookFile : ExtentHookBase {
	public:
		ExtentHookFile(std::string file_backing) : ExtentHookBase(), _file_backing(file_backing) {}
	private:
		std::string _file_backing;
};

class ExtentHookSpilling : ExtentHookBase {
	public:
		ExtentHookSpilling() {}
};

class ExtentHook: public ExtentHookBase {
	public:
		ExtentHook();

		void * ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				size_t alignment, bool *zero, bool *commit, unsigned arena_id);

		bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		void ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				bool committed, unsigned arena_id);

		bool ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookPurge(extent_hooks_t *extent_hooks, void *addr, size_t size,
				size_t offset, size_t length, unsigned arena_id);

		bool ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
				size_t size_b, bool committed, unsigned arena_id);

		bool ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				size_t size_b, bool committed, unsigned arena_id);
};

}