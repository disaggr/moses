#pragma once

#include "extent_hook_base.h"

namespace moses {

// arenas can be
// fixed: bound to a NUMA node with no means to spill, but be subject to paging
// home: have a fixed home node but may spill to other places as well
// no paging:
// low priority
// 

class Arena {
	public:
		Arena(unsigned arena, ExtentHookBase *hook);
		Arena(unsigned arena, ExtentHookBase *hook, int memory_protection_key);
		
		unsigned GetId() {
			return _arena;
		}

		ExtentHookBase* GetHook() {
			return _hook;
		};

	private:
		unsigned _arena;
		int _memory_protection_key;
		ExtentHookBase* _hook;
};

}