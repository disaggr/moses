#include "extent_hook.h"
#include <iostream>

namespace moses {

void * ExtentHook::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
	                                        size_t alignment, bool *zero, bool *commit, unsigned arena_id) {
    std::cout << "Implement me" << std::endl;
	void *ret = ExtentHookBase::ExtentHookAlloc(extent_hooks, new_addr, size, alignment, zero, commit, arena_id);
	return ret;
}

bool ExtentHook::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

void ExtentHook::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
}

bool ExtentHook::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
	                                        size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool ExtentHook::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                    	size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool ExtentHook::ExtentHookPurge(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            				size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool ExtentHook::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
    				                        size_t size_b, bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool ExtentHook::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
				                            size_t size_b, bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}
}