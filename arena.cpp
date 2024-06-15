#include "arena.h"
#include <iostream>

namespace moses {

Arena::Arena() {
}

void* Arena::ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
				                        size_t alignment, bool *zero, bool *commit, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return NULL;
}

bool Arena::ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size,
				                        bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

void Arena::ExtentHookDestroy(extent_hooks_t *extent_hooks, void *addr, size_t size,
				                        bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
}

bool Arena::ExtentHookCommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
		                        		size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool Arena::ExtentHookDecommit(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            			size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool Arena::ExtentHookPurgeLazy(extent_hooks_t *extent_hooks, void *addr, size_t size,
                            				size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool Arena::ExtentHookPurgeForced(extent_hooks_t *extent_hooks, void *addr, size_t size,
                                        	size_t offset, size_t length, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool Arena::ExtentHookSplit(extent_hooks_t *extent_hooks, void *addr, size_t size, size_t size_a,
                                	size_t size_b, bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

bool Arena::ExtentHookMerge(extent_hooks_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b,
                				size_t size_b, bool committed, unsigned arena_id) {
	std::cout << "Implement me" << std::endl;
	return false;
}

}