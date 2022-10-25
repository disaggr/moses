#pragma once

#include <pthread.h>
#include <jemalloc.h>

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

class PlaceGuardStack {
	public:
		PlaceGuardStack() {
			(void) pthread_once(&key_once, Initialize());
			arenas = new std::vector<unsigned>();
			(void) pthread_setspecific(pg_stack_key, this);
		}

		void Push(unsigned arena) {
			arenas->push(arena);
			size_t size = sizeof(arena);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
		}

		unsigned Pop() {
			unsigned arena = arenas->back();
			arenas->pop_back();
			return arena;
		}
	private:
		Initialize() {
			(void) pthread_key_create(&pg_stack_key, NULL);
		}
		std::vector<unsigned> arenas;
}

class PlaceGuard {
	public:
		PlaceGuard(const unsigned arena) {
			_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena);
		}

		~PlaceGuard() {
		       _pg_stack->pop();
	       }
	private:
		PlaceGuardStack *_pg_stack;
}

class IsolatedPlaceGuard {
}

class ExtentHookBase {
	public:
		bool ExtentHookAlloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size, size_t alignment, bool *zero, bool commit, unsigned arena_ind)
		bool ExtentHookDAlloc(extent_hooks_t *extent_hooks, void *addr, size_t size, bool committed, unsigned arena_ind)
		bool ExtentHookDestroy(extent_hook_t *extent_hooks, void *addr, size_t size, bool committed, unsigned arena_id)
		bool ExtentHookCommit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t offset, size_t length, unsigned arena_id)
		bool ExtentHookDecommit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t offset, size_t length, unsigned arena_id)
		bool ExtentHookPurge(extent_hook_t *extent_hooks, void *addr, size_t size, size_t offset, size_t length, unsigned arena_id)
		bool ExtentSplit(extent_hook_t *extent_hooks, void *addr, size_t size, size_t size_a, size_t size_b, bool committed, unsigned arena_id)
		bool ExtentMerge(extent_hook_t *extent_hooks, void *addr_a, size_t size_a, void *addr_b, size_t size_b, bool committed, unsigned arena_id)
}

class ExtendHookNuma {
	public:
		ExtendHookNuma(int node) : _node(node) { }
		bool extent_dalloc_hook(extent_hooks_t *extent_hooks, void *addr, size_t size, bool committed, unsigned arena_ind)
	private:
		int _node;
}

class ExtendHookFile {
	public:
		ExtendHookFile(std::string file_backing) : _file_backing(file_backing) {
		}
	private:
		std::string _file_backing;
}

class ExtendHookSpilling {
}

enum Places {
	local,
	remote,
	disaggregated,
	dontcare
}

class Moses {
	public:
		static Moses& GetInstance() {
			if(_instance == NULL) {
				_instance = new Moses();
			}
			return _instance;
		}

		void Initialize() {
			unsigned arena_ind;
			size_t unsigned_sz = sizeof(unsigned);
			extent_hooks_t hooks;
			size_t hooks_sz = sizeof(extent_hooks_t);
			mallctl("arenas.create", (void *) &arena_ind, sz, (void *) hooks, hooks_sz);	
		}

		unsigned GetArena(Places p) {

		}
	private:
		Moses() {}
		Moses *_instance;
		std::vector 
}

}
