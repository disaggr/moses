#pragma once

#include <pthread.h>
#include <jemalloc.h>
#include <numa.h>
#include <linux/getcpu.h>

namespace moses {

static pthread_once_t key_once;
static pthread_key_t pg_stack_key;

class Arena {
	public:
		Arena(unsigned arena, int memory_protection_key, ExtentHookBase hook)
	private:
		unsigned _arena;
		int _memory_protection_key;
		ExtentHookBase _hook;
}

class PlaceGuardStack {
	public:
		PlaceGuardStack() {
			(void) pthread_once(&key_once, Initialize());
			arenas = new std::vector<unsigned>();
			(void) pthread_setspecific(pg_stack_key, this);
			unsigned current_arena;
			size_t arena_size = sizeof(unsigned);
			mallctl("thread.arena", (void *) &current_arena, &size, NULL, NULL);
			Push(current_arena);
		}

		void Push(unsigned arena) {
			Push(arena, false);
		}

		void Push(unsigned arena, bool isolated) {
			std::pair<unsigned,bool> arena_pair = arenas->back();
			if(isolated == false && arena_pair.second == true) {
				//Error this is forbidden
			}
			arenas->push(std::make_pair(arena, isolated));
			size_t size = sizeof(unsigned);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
		}

		unsigned Pop() {
			unsigned last_arena = arenas->back();
			arenas->pop_back();
			unsigned arena = arenas->back();
			size_t size = sizeof(unsigned);
			mallctl("thread.arena", NULL, NULL, (void *) &arena, &size);
			return last_arena;
		}
	private:
		Initialize() {
			(void) pthread_key_create(&pg_stack_key, NULL);
		}
		std::vector<std::pair<unsigned,bool>> arenas;
}

class PlaceGuard {
	public:
		PlaceGuard(const unsigned arena) {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena);
		}

		~PlaceGuard() {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			unsigned arena = _pg_stack->pop();
			// give arena back to moses?
	       }
}

class IsolatedPlaceGuard {
	//TODO: tcache disable or flush to make sure, no leakage is done in the thread caches
	//see https://github.com/jemalloc/jemalloc/issues/1016
	public:
		IsolatedPlaceGuard(const unsigned arena) {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			_pg_stack->push(arena, true);
		}

		~IsolatedPlaceGuard() {
			PlaceGuardStack *_pg_stack = pthread_getspecific(pg_stack_key);
			unsigned arena = _pg_stack->pop();
		}
}

class ExtentHookBase {
	public:
		ExtendHookBase() {
			size_t sz = sizeof(extent_hooks_t);
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

class ExtendHookFile {
	public:
		ExtendHookFile(std::string file_backing) : _file_backing(file_backing) {
		}
	private:
		std::string _file_backing;
}

class ExtendHookSpilling {
}

enum Place {
	fast,
	parallel,
	capacity,
	may_spill,
	explicit_place,
	dontcare
}

class Place {
	public:
		Place(std::string name, Place p) : _name(name), _place(place)
	private:
		std::string _name;
		Place _place;
}

class ExtentHookDispatch {
//TODO: Implement the required jemalloc hooks and dispatch them according to the arena_id to the respective objects
}

class Moses {
	public:
		static Moses& GetInstance() {
			if(_instance == NULL) {
				_instance = new Moses();
			}
			return _instance;
		}

		void CreateArena(ExtentHookBase &custom_hooks) {
			unsigned arena_ind;
			size_t unsigned_sz = sizeof(unsigned);
			extent_hooks_t hooks;
			//need to create a generic extent_hooks_t with function pointers to a static class
			//this class then asks moses to dispatch depending on the arena id
			hooks.alloc = custom_hooks.ExtentHookAlloc;
			hooks.dalloc = custom_hooks.
			hooks.destroy = custom_hooks.
			hooks.commit = custom_hooks.
			hooks.decommit = custom_hooks.
			hooks.purge_lazy = custom_hooks.
			hooks.purge_forced = custom_hooks.
			hooks.split = custom_hooks.
			hooks.merge = custom_hooks.
			size_t hooks_sz = sizeof(extent_hooks_t);
			mallctl("arenas.create", (void *) &arena_ind, sz, (void *) hooks, hooks_sz);	
		}

		unsigned GetArena(Places p) {
			switch(p) {
				case fast:
					unsigned *cpu, *node;
					int error = getcpu(cpu, node, NULL);
					if(error)
						exit(-1);
					ExtendHookNuma(*node);
					CreateArena
					break;
			}
		}
	private:
		Moses() {}
		Moses *_instance;
		std::map<Place, std::vector<unsigned>> arena_cache;
		std::map<unsigned, 
}

}
